#pragma once

#include "Interfaces/IMainState.h"
#include "ConsoleHandlers/InputHandler.h"
#include "Console.h"
#include "PageManager.h"

class LauncherPageManager : public PageManager {
public:
    LauncherPageManager(IInput& input, std::function<void()> startGameCallback);
    ~LauncherPageManager();

};
