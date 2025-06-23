#pragma once

#include "../Core/DefaultPage.h"
#include "Game.h"
#include <mutex>

class GameMainPage : public DefaultPage, public ILogic {
    GAME game;
    std::mutex gameMutex;
    Player* player;
public:
    GameMainPage(IInput& input);
    ~GameMainPage();

    ILogic* getLogic() override;
    void update(float deltaTime) override;
    void render(ScreenHandler& screenHandler) override;
};