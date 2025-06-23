#include "ButtonObject.h"
#include "Console.h"

ButtonObject::ButtonObject()
{

}

ButtonObject::ButtonObject(int x, int y, const std::wstring& label)
    : TextObject(x, y, label) {}

ButtonObject::ButtonObject(int x, int y, const std::wstring& label, std::function<void()> callback)
    : TextObject(x, y, label), onClickCallback(callback){}

void ButtonObject::onHover(bool isHovered) {
    hovered = isHovered;
    //Optionally change appearance, play sound, etc.
}

void ButtonObject::setOnClick(std::function<void()> callback) {
    onClickCallback = callback;
}

void ButtonObject::onClick() {
    if (onClickCallback)
        onClickCallback();
}

void ButtonObject::update(float deltaTime)
{
    
}

bool ButtonObject::inside(COORD mouse) const {
    return mouse.X/2 >= x && mouse.X/2 <= x + width &&
           mouse.Y >= y && mouse.Y <= y + height;
}

void ButtonObject::registerInputListener(IInput& input) {
    input.addMouseClickListener([this](const MouseClickEvent& ev) {
        if (inside(ev.position)) {
            onClick();
        }
    });
}