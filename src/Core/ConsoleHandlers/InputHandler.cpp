#include "InputHandler.h"
#include <iostream>

InputHandler::InputHandler()
{
    mousePos = {0,0};
}

InputHandler::~InputHandler()
{
    
}

void InputHandler::enqueueMouseClick(const MouseClickEvent& event) {
    {
        std::lock_guard<std::mutex> lock(mouseEventMutex);
        mouseClickQueue.push(event);
    }

    // надсилаємо подію всім слухачам
    for (auto& listener : mouseClickListeners) {
        listener(event);
    }
}

void InputHandler::addMouseClickListener(std::function<void(const MouseClickEvent&)> listener) {
    mouseClickListeners.push_back(listener);
}

void InputHandler::enqueueKeyPress(const KeyPressEvent& event) {
    {
        std::lock_guard<std::mutex> lock(keyEventMutex);
        keyPressQueue.push(event);
    }

    for (auto& listener : keyPressListeners) {
        listener(event);
    }
}

void InputHandler::addKeyPressListener(std::function<void(const KeyPressEvent&)> listener) {
    keyPressListeners.push_back(listener);
}