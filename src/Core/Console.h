#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include <fcntl.h>
#include <io.h>

#include "UI/Pixel.h"

#include "ConsoleHandlers/InputHandler.h"
#include "ConsoleHandlers/ScreenHandler.h"
#include "ConsoleHandlers/LogicHandler.h"

class Console
{
    HANDLE hInput;
    HANDLE hOutput;
    HWND hwnd;
    DWORD mode;

    ScreenHandler screenHandler;
    LogicHandler logicHandler;

public:
    Console();
    ~Console();
    ScreenHandler& getScreenHandler() { return screenHandler; }
    LogicHandler& getLogicHandler() { return logicHandler; }
};
