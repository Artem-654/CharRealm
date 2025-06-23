#pragma once

#include <windows.h>
#include <functional>

struct MouseClickEvent {
    COORD position;
    DWORD button;
};

struct KeyPressEvent {
    wchar_t keyChar;
    WORD virtualKeyCode;
};

class IInput {
public:
    virtual void enqueueMouseClick(const MouseClickEvent& event) = 0;
    virtual void addMouseClickListener(std::function<void(const MouseClickEvent&)> listener) = 0;

    virtual void enqueueKeyPress(const KeyPressEvent& event) = 0;
    virtual void addKeyPressListener(std::function<void(const KeyPressEvent&)> listener) = 0;

    virtual ~IInput() {}
};
