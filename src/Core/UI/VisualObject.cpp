#include "VisualObject.h"

VisualObject::VisualObject()
{
    
}

VisualObject::VisualObject(int x, int y) : x(x), y(y)
{
    width = 3;
    height = 3;
    border = 1;
}

void VisualObject::render(ScreenHandler& screenHandler)
{
    //screenHandler.;
}

void VisualObject::update(float deltaTime)
{

}