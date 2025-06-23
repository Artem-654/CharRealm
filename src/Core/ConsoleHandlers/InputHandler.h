#pragma once

#include <windows.h>
#include <set>
#include <queue>
#include <mutex>
#include <vector>
#include "../Interfaces/IInput.h"
#include <functional>

class InputHandler : public IInput {
private:
    std::queue<MouseClickEvent> mouseClickQueue;
    std::mutex mouseEventMutex;
    std::vector<std::function<void(const MouseClickEvent&)>> mouseClickListeners;

    //std::set<int> pressedKeys;
    //std::set<int> pressedMouseButtons;
    COORD mousePos;

    std::queue<KeyPressEvent> keyPressQueue;
    std::mutex keyEventMutex;
    std::vector<std::function<void(const KeyPressEvent&)>> keyPressListeners;

public:

    InputHandler();
    
    void enqueueMouseClick(const MouseClickEvent& event);
    void addMouseClickListener(std::function<void(const MouseClickEvent&)> listener);

    void enqueueKeyPress(const KeyPressEvent& event);
    void addKeyPressListener(std::function<void(const KeyPressEvent&)> listener);

    ~InputHandler();
};