#include "ButtonHandler.hpp"
#include "Screen.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

ButtonHandler::ButtonHandler(float cooldownTime) :
    lastPressTimes(std::vector<float>(GLFW_KEY_LAST + 1, 0.0f)),
    cooldownTime(cooldownTime)
{}

bool ButtonHandler::isPressed(int GLFW_KEY) {
    GLFWwindow* window = Screen::getInstance().window;
    if (!(glfwGetKey(window, GLFW_KEY) == GLFW_PRESS)) return false;
    float lastPressTime = lastPressTimes[GLFW_KEY];
    if (glfwGetTime() - lastPressTime > cooldownTime) {
        lastPressTimes[GLFW_KEY] = glfwGetTime();
        return true;
    }
    return false;
}