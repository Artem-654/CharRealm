#include "MainMenuPage.h"
#include "UI/VisualObject.h"
#include "UI/ButtonObject.h"
#include "ConsoleHandlers/InputHandler.h"
#include <vector>
#include <iostream>

MainMenuPage::MainMenuPage(IInput& input, std::function<void()> startGameCallback) : DefaultPage(input), onStartGame(startGameCallback) {
    VsOb.push_back(std::make_shared<TextObject>(5, 3, L" CharRealm "));
    VsOb.push_back(std::make_shared<ButtonObject>(5, 15, L" Start game ",[this](){
        if (onStartGame) onStartGame();
    }));
    VsOb.push_back(std::make_shared<ButtonObject>(5, 20,L" Exit ",[this](){
        exitRequested = true;
    }));

    for (auto& obj : VsOb) {
        auto btn = std::dynamic_pointer_cast<ButtonObject>(obj);
        if (btn) {
            btn->registerInputListener(input);
        }
    }    
}