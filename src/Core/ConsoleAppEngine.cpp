#include "ConsoleAppEngine.h"
#include "PageManagers/LauncherPageManager.h"
#include "PageManagers/GamePageManager.h"
#include <iostream>

ConsoleAppEngine:: ConsoleAppEngine() {
    consolePtr = new Console();
    inputPtr = new InputHandler();
}

ConsoleAppEngine::~ConsoleAppEngine() {
    threadManager.stop();
    threadManager.join();
    delete inputPtr;
    delete currentStatePtr;
    delete consolePtr;
}

void ConsoleAppEngine::run() {

    bool switchToGameRequested = false;
    auto requestGame = [&]() { switchToGameRequested = true; };

    currentStatePtr = new LauncherPageManager(*inputPtr, requestGame);

    threadManager.run(*currentStatePtr, *consolePtr);

    while (!currentStatePtr->shouldExit()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        if (switchToGameRequested) {
            threadManager.stop();
            threadManager.join();

            delete currentStatePtr;
            currentStatePtr = new GamePageManager(*inputPtr);
            threadManager.run(*currentStatePtr, *consolePtr);

            switchToGameRequested = false;
        }
    }

    threadManager.stop();
    threadManager.join();
}