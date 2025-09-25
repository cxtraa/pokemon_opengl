#pragma once

#include <nlohmann/json.hpp>
#include "Pokemon.hpp"

using json = nlohmann::json;

class Trainer {
public:
    Trainer(const json& data);
    std::vector<std::unique_ptr<Pokemon>> pokemon;
    int currMonIdx;
    Pokemon* currMon();
    std::string name;
    bool isDead();
};