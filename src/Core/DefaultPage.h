#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <fcntl.h>
#include <io.h>
#include "Console.h"
#include "Interfaces/IMainState.h"
#include "ConsoleHandlers/InputHandler.h"
#include "UI/VisualObject.h"
#include "UI/TextObject.h"
#include "UI/ButtonObject.h"

class DefaultPage : public IMainState, public IRenderable
{
protected:
    IInput& input;
    bool exitRequested = false;
    std::vector<std::shared_ptr<VisualObject>> VsOb;


public:
    DefaultPage(IInput& input);
    ~DefaultPage();

    IInput* getIInput() override;
    ILogic* getLogic() override;
    IRenderable* getRenderer() override;
    bool shouldExit() const override;

    void render(ScreenHandler& screenHandler);
};