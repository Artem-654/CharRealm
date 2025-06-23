#include "GamePageManager.h"
#include "../../Game/GameMainPage.h"

GamePageManager::GamePageManager(IInput& input) : PageManager(input)
{
    currentPage = new GameMainPage(input);
}

GamePageManager::~GamePageManager()
{
    
}
