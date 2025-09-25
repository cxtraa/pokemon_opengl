#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>
#include <stdexcept>
#include "Move.hpp"
#include "Stat.hpp"
#include "Status.hpp"
#include <unordered_set>

using json = nlohmann::json;

class Pokemon {
private:
    int longCalc(int dv, int ev, int level, int base);
public:
    const static std::vector<float> criticalMultipliers;

    int id; 
    std::string name;
    std::string nickname;
    int hp;
    int level;
    int critHitStage;
    std::unordered_set<Status> statuses;
    std::vector<int> EVs;
    std::vector<int> DVs;
    std::vector<int> stats();

    std::vector<int> statStages;
    std::vector<int> baseStats;
    std::vector<json> moves;

    void logPokemon();

    int damageDone(Pokemon* attacker, Pokemon* defender, int moveIdx);
    void useMove(Pokemon* enemy, int moveIdx);
    void useDmgingMove(Pokemon* enemy, int moveIdx);
    void useStatusMove(Pokemon* enemy, int moveIdx);

    void applyStatStageChange(Stat stat, int change);
    void applyTurn(Pokemon* enemy, int moveIdx);

    bool hasStatus(Status status) const;
    int confusionDamageDone(Pokemon* attacker, Pokemon* defender);
    
    const std::vector<std::string> getMovesAsStrVec() const;
    static Stat strToStat(std::string stat);
    static std::string statToStr(Stat stat);

    Pokemon(std::string name, std::string nickname, int level);
};