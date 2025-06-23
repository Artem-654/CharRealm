#include "Console.h"

Console::Console() :
    hInput(GetStdHandle(STD_INPUT_HANDLE)),
    hOutput(GetStdHandle(STD_OUTPUT_HANDLE)),
    hwnd(GetConsoleWindow())
{
    //SetConsoleOutputCP(CP_UTF8);
    //enableVTMode();


    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin),  _O_U16TEXT);

    DWORD inputMode = 0;
    GetConsoleMode(hInput, &inputMode);
    SetConsoleMode(hInput, (inputMode | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE);

    DWORD outputMode = 0;
    GetConsoleMode(hOutput, &outputMode);
    SetConsoleMode(hOutput, outputMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    SetConsoleTitleW(L"CharRealm");
    ShowWindow(hwnd, SW_MAXIMIZE);

    screenHandler.setMaxScreenSizeToCurrent();
    screenHandler.resizeIfNeeded();

    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(hOutput, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOutput, &cursorInfo);

}

Console::~Console()
{

}
