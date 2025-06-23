#pragma once

#include "../ConsoleHandlers/ScreenHandler.h"

class VisualObject
{
protected:
    int x, y, width, height, border;
public:
    VisualObject();
    VisualObject(int x, int y);
    virtual void render(ScreenHandler& screenHandler);
    virtual void update(float deltaTime);
    virtual ~VisualObject() = default;
};