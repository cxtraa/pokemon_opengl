#include <iostream>
#include "Pokemon.hpp"
#include "Trainer.hpp"
#include "Battle.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main() {
    std::ifstream file("./trainers.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open trainers.json" << std::endl;
    }
    json j;
    file >> j;
    const json playerData = j["player"];
    const json trainerData = j["trainer_1"];
    
    Trainer player(playerData);
    Trainer trainer(trainerData);
    
    Battle battle(&player, &trainer);

    battle.battle();
    return 0;
}