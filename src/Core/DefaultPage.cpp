#include "DefaultPage.h"

DefaultPage::DefaultPage(IInput& input) : input(input), exitRequested(false) {}

DefaultPage::~DefaultPage() {}

IInput* DefaultPage::getIInput() {
    return &input;
}

ILogic* DefaultPage::getLogic() {
    return nullptr;
}

IRenderable* DefaultPage::getRenderer() {
    return this; // реалізуємо render() всередині DefaultPage
}

bool DefaultPage::shouldExit() const {
    return exitRequested;
}

void DefaultPage::render(ScreenHandler& screenHandler) {
    screenHandler.resizeIfNeeded();
    screenHandler.clear();
    for (auto& obj : VsOb)
        obj->render(screenHandler);
}