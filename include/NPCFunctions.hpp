#pragma once

#include "NPC.hpp"
#include <unordered_map>
#include <functional>

using NPCFunc = std::function<void(NPC*)>;

class NPCFunctions {
public:
    static std::unordered_map<std::string, NPCFunc>& getRegistry();
    static void invoke(NPC* npc, std::string npcFuncName);
    
    // NPC functions
    static void npcDefault(NPC* npc);
};