#pragma once

#include "IInput.h"
#include "ILogic.h"
#include "IRenderable.h"

class IMainState {
public:
    virtual bool shouldExit() const = 0;
    virtual IInput* getIInput() = 0;
    virtual ILogic* getLogic() = 0;
    virtual IRenderable* getRenderer() = 0;
};