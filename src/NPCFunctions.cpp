
#include <iostream>
#include "NPCFunctions.hpp"
#include "Game.hpp"
#include "Direction.hpp"

std::unordered_map<std::string, NPCFunc>& NPCFunctions::getRegistry() {
    static std::unordered_map<std::string, NPCFunc> registry = [](){
        std::unordered_map<std::string, NPCFunc> reg;

        reg["npcDefault"] = npcDefault;
        return reg;
    }();
    return registry;
}

void NPCFunctions::invoke(NPC* npc, std::string funcName) {
    auto& registry = getRegistry();
    if (registry.find(funcName) != registry.end()) {
        registry[funcName](npc);
    }
    else {
        throw std::runtime_error("NPCFunctions: the onInteract function " + funcName + " does not exist in the registry.");
    }
}

// NPC FUNCTIONS GO HERE
void NPCFunctions::npcDefault(NPC* npc) {
    // The NPC should turn to face the user
    Player& player = Game::getInstance().player;
    if (player.tileY == npc->tileY - 1) {
        npc->direction = Direction::UP;
    }
    else if (player.tileX == npc->tileX + 1) {
        npc->direction = Direction::RIGHT;
    }
    else if (player.tileY == npc->tileY + 1) {
        npc->direction = Direction::DOWN;
    }
    else {
        npc->direction = Direction::LEFT;
    }
    // Display the dialogue
    DialogueManager& dialogueManager = Game::getInstance().dialogueManager;
    dialogueManager.display(npc->dialogue);
}