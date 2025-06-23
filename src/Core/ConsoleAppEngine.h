#pragma once

#include "Console.h"
#include "Interfaces/IMainState.h"
#include "ThreadManager.h"

class ConsoleAppEngine {
private:
    Console* consolePtr;
    IMainState* currentStatePtr;
    ThreadManager threadManager;

    IInput* inputPtr;

public:
    ConsoleAppEngine();
    ~ConsoleAppEngine();

    void run();
};
