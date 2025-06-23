#include "GameMainPage.h"
#include "UI/VisualObject.h"
#include "UI/ButtonObject.h"
#include "ConsoleHandlers/InputHandler.h"
#include <vector>
#include <iostream>

GameMainPage::GameMainPage(IInput& input) : DefaultPage(input), game() {
    //VsOb.push_back(std::make_shared<TextObject>(5, 3, L" CharRealm Started "));

    // for (auto& obj : VsOb) {
    //     auto btn = std::dynamic_pointer_cast<ButtonObject>(obj);
    //     if (btn) {
    //         btn->registerInputListener(input);
    //     }
    // }    
    player = game.getPLayerPtr();

    input.addKeyPressListener([this](const KeyPressEvent& ev) {
        player->action(ev.keyChar);
});
}

GameMainPage::~GameMainPage()
{
    
}

ILogic* GameMainPage::getLogic() {
    return this;
}

void GameMainPage::update(float deltaTime)
{
    std::lock_guard<std::mutex> lock(gameMutex);
    
    game.update();
    game.setFromBufferToMap();
}

void GameMainPage::render(ScreenHandler& screenHandler)
{
    std::lock_guard<std::mutex> lock(gameMutex);

    screenHandler.resizeIfNeeded();

    game.setFromPlayerScreenPos();
    game.showScreen(screenHandler);
}
