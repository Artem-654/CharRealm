#include "PageManager.h"

PageManager::PageManager(IInput& input) : input(input)
{

}

PageManager::~PageManager() = default;

IInput* PageManager::getIInput() {
    return currentPage ? currentPage->getIInput() : nullptr;
}

ILogic* PageManager::getLogic() {
    return currentPage ? currentPage->getLogic() : nullptr;
}

IRenderable* PageManager::getRenderer() {
    return currentPage ? currentPage->getRenderer() : nullptr;
}

bool PageManager::shouldExit() const {
    return currentPage->shouldExit();
}