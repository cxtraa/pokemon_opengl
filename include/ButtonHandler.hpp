#pragma once
#include <vector>

class ButtonHandler {
public:
    std::vector<float> lastPressTimes;
    float cooldownTime;
    ButtonHandler(float cooldownTime);
    bool isPressed(int GLFW_KEY);
};