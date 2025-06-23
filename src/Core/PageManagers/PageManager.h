#pragma once

#include "Interfaces/IMainState.h"
#include "ConsoleHandlers/InputHandler.h"
#include "Console.h"

class PageManager : public IMainState {
protected:
    IMainState* currentPage;
    IInput& input;

public:
    PageManager(IInput& input);
    ~PageManager();

    IInput* getIInput() override;
    ILogic* getLogic() override;
    IRenderable* getRenderer() override;
    bool shouldExit() const override;

};
