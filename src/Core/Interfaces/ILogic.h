#pragma once

class ILogic {
public:
    virtual ~ILogic() = default;
    virtual void update(float deltaTime) = 0;
};