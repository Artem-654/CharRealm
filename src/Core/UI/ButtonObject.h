#pragma once
#include "VisualObject.h"
#include "TextObject.h"
#include <functional>
#include <windows.h>
#include "../Interfaces/IInput.h"

class ButtonObject : public TextObject 
{
public:
    ButtonObject();
    ButtonObject(int x, int y, const std::wstring& label);
    ButtonObject(int x, int y, const std::wstring& label, std::function<void()> callback);

    void setOnClick(std::function<void()> callback);
    
    void update(float deltaTime) override;
    void registerInputListener(IInput& input);
private:
    void onClick();
    void onHover(bool isHovered);
    bool inside(COORD mouse) const;
    std::function<void()> onClickCallback;
    bool hovered;
};
