#pragma once

#include "../../Core/DefaultPage.h"

class MainMenuPage : public DefaultPage {
private:
    std::function<void()> onStartGame;
public:
    MainMenuPage(IInput& input, std::function<void()> startGameCallback);
};