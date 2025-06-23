#include "LauncherPageManager.h"
#include "../../Launcher/Pages/MainMenuPage.h"

LauncherPageManager::LauncherPageManager(IInput& input, std::function<void()> startGameCallback) : PageManager(input)
{
    currentPage = new MainMenuPage(input, startGameCallback);
}

LauncherPageManager::~LauncherPageManager()
{
    
}
