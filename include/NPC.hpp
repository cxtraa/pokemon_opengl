
#pragma once

#include "Character.hpp"

class NPC : public Character {
public:
    std::string onInteractFuncName;
    std::vector<int> movements;
    int movementIdx;
    std::string dialogue;
    NPC(std::string characterName, int x, int y, std::string onInteractFuncName, std::vector<int> movements, std::string dialogue);
    void updateMove();
};