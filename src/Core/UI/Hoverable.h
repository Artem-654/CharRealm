#pragma once

class Hoverable
{
protected:
    bool hoveredState;
public:
    Hoverable() { hoveredState = false; };
    ~Hoverable();
    virtual void onHover(bool hovered) { hoveredState = hovered; };
};
