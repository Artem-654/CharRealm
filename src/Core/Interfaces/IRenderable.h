#pragma once

class ScreenHandler;

class IRenderable {
public:
    virtual ~IRenderable() = default;
    virtual void render(ScreenHandler& screenHandler) = 0;
};