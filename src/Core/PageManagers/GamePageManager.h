#pragma once

#include "Interfaces/IMainState.h"
#include "ConsoleHandlers/InputHandler.h"
#include "Console.h"
#include "PageManager.h"

class GamePageManager : public PageManager {
public:
    GamePageManager(IInput& input);
    ~GamePageManager();

};
