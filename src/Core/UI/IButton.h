#pragma once

class IButton {
public:
    virtual void onClick() = 0;
    virtual ~IButton() = default;
};