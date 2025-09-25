
#include "NPC.hpp"

NPC::NPC(std::string characterName, int x, int y, std::string onInteractFuncName, std::vector<int> movements, std::string dialogue) :
    Character(characterName, x, y),
    onInteractFuncName(onInteractFuncName),
    movements(movements),
    dialogue(dialogue)
{}

void NPC::updateMove() {
    if (movements.size() == 0) return;
    // Do the current movement specified
    if (!moving) {
        if (move(static_cast<Direction>(movementIdx))) {
            movementIdx = (movementIdx + 1) % movements.size();
        }
    }
}