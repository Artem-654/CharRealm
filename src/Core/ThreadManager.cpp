#include "ThreadManager.h"
#include "Interfaces/IMainState.h"
#include <chrono>
#include <iostream>

ThreadManager::ThreadManager() : running(true) {}

ThreadManager::~ThreadManager() {
    stop();
    join();
}

void ThreadManager::run(IMainState& state, Console& console) {
    running = true;

    if (ILogic* logic = state.getLogic()) {
        logicThread = std::thread([this, logic]() {
            while (running) {
                logic->update(0.016f);
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }
        });
    }

    if (IRenderable* renderer = state.getRenderer()) {
        ScreenHandler& screenHandler = console.getScreenHandler();
        renderThread = std::thread([this, renderer, &screenHandler]() {
            while (running) {
                renderer->render(screenHandler);
                screenHandler.draw();
                std::this_thread::sleep_for(std::chrono::milliseconds(16));
            }
        });
    }

    if (IInput* input = state.getIInput()) {
        inputThread = std::thread([this, input]() {
            HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
            DWORD events;
            INPUT_RECORD inputRecord;

            while (running) {
                if (ReadConsoleInput(hInput, &inputRecord, 1, &events)) {
                    if (inputRecord.EventType == MOUSE_EVENT) {
                        const auto& mouseEvent = inputRecord.Event.MouseEvent;
                        if (mouseEvent.dwEventFlags == 0 && mouseEvent.dwButtonState != 0) {
                            MouseClickEvent ev{ mouseEvent.dwMousePosition, mouseEvent.dwButtonState };
                            input->enqueueMouseClick(ev);
                        }
                    }
                    else if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
                        const auto& keyEvent = inputRecord.Event.KeyEvent;
                        KeyPressEvent ev {
                            keyEvent.uChar.UnicodeChar,
                            keyEvent.wVirtualKeyCode
                        };
                        input->enqueueKeyPress(ev);
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(8));
            }
        });
    }
}

void ThreadManager::stop() {
    running = false;
}

void ThreadManager::join() {
    if (inputThread.joinable()) inputThread.join();
    if (logicThread.joinable()) logicThread.join();
    if (renderThread.joinable()) renderThread.join();
}
