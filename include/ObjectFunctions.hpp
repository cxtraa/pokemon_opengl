#pragma once

#include "Object.hpp"
#include "Game.hpp"

#include <functional>
#include <unordered_map>

using ObjectFunc = std::function<void(Object*)>;

class ObjectFunctions {
public:
    static std::unordered_map<std::string, ObjectFunc>& getRegistry();
    static void invoke(Object* object, std::string funcName);

    static void pickUpItem(Object* object);
};