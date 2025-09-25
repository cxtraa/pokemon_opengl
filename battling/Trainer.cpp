
#include "Trainer.hpp"

Trainer::Trainer(const json& data) : currMonIdx(0) {
    name = data["name"].get<std::string>();
    for (const json& monData : data["pokemon"]) {
        std::string name = monData["name"];
        std::string nickname = monData["nickname"];
        int level = monData["level"];
        std::unique_ptr<Pokemon> monPtr = std::make_unique<Pokemon>(name, nickname, level);
        pokemon.emplace_back(std::move(monPtr));
    }
}

Pokemon* Trainer::currMon() {
    return pokemon[currMonIdx].get();
}

bool Trainer::isDead() {
    for (auto& mon : pokemon) {
        if (!mon->hasStatus(Status::FAINT)) return false;
    }
    return true;
}