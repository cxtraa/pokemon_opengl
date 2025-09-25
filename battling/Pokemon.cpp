
#include "Utils.hpp"
#include "Pokemon.hpp"
#include "Stat.hpp"

#include <limits>
#include <unordered_map>
#include <cmath>
#include <algorithm>

const std::vector<float> Pokemon::criticalMultipliers = {1.0f/512.0f, 1.0f/64.0f, 1.0f/8.0f, 1.0f/2.0f, 100.0f};

Pokemon::Pokemon(std::string name, std::string nickname, int level) :
    id(Utils::randInt(0, std::numeric_limits<int>::max())),
    nickname(nickname),
    critHitStage(0),
    level(level),
    name(name),
    EVs(std::vector<int>(6, 0)),
    DVs(std::vector<int>(6, 0)),
    baseStats(std::vector<int>(6, 0)),
    statStages(std::vector<int>(Stat::NUM_STATS, 0))
{
    if (nickname.length() == 0) this->nickname = name;
    std::transform(
        name.begin(),
        name.end(),
        name.begin(),
        [](const char& c) {
            return std::tolower(c);
        }
    );
    // Load pokemon data json
    std::ifstream file("./gen2_pokemon_crystal.json");
    json pokemonData;
    file >> pokemonData;

    std::ifstream file2("./gen2_crystal_all_moves.json");
    json movesData;
    file2 >> movesData;

    if (pokemonData.find(name) == pokemonData.end()) {
        throw std::runtime_error("Was unable to find the Pokemon " + name + " in the JSON.");
    }

    // Populate DVs with random integers in [0, 15]
    for (int i = 0; i < 6; i++) {
        DVs[i] = Utils::randInt(0, 15);
    }

    // Obtain a random selection of 4 moves out of the available ones
    const std::vector<json>& movesUseableJson = pokemonData[name]["learnsets"]["levelUp"].get<std::vector<json>>();
    std::vector<std::string> movesUseable;
    int moveLevel = 1;
    for (int i = 0; i < movesUseableJson.size(); i++) {
        std::string moveName = movesUseableJson[i]["move"].get<std::string>();
        int levelLearnt = movesUseableJson[i]["level"].get<int>();
        if (levelLearnt > level) {
            break;
        }
        movesUseable.push_back(moveName);
    }

    std::vector<int> randomIndices = Utils::chooseK(4, movesUseable.size());
    for (int idx : randomIndices) {
        std::string moveName = movesUseable[idx];
        json move = movesData[moveName];
        move["name"] = moveName;
        moves.push_back(move);
    }

    json baseStatsJson = pokemonData[name]["baseStats"].get<json>();
    baseStats[Stat::HP] = baseStatsJson["hp"].get<int>();
    baseStats[Stat::ATTACK] = baseStatsJson["attack"].get<int>();
    baseStats[Stat::DEFENSE] = baseStatsJson["defense"].get<int>();
    baseStats[Stat::SPECIAL_ATTACK] = baseStatsJson["special-attack"].get<int>();
    baseStats[Stat::SPECIAL_DEFENSE] = baseStatsJson["special-defense"].get<int>();
    baseStats[Stat::SPEED] = baseStatsJson["speed"].get<int>();

    hp = stats()[Stat::HP];
}

void Pokemon::logPokemon() {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Nickname: " << nickname << std::endl;
    std::cout << "Current HP: " << hp << std::endl;
    std::cout << "EVs: ";
    Utils::logVector(EVs, ",");
    std::cout << "DVs: ";
    Utils::logVector(DVs, ",");
    std::cout << "Stats: ";
    Utils::logVector(stats(), ",");

    std::cout << "Moves: ";
    Utils::logVector(getMovesAsStrVec(), ",");
}

const std::vector<std::string> Pokemon::getMovesAsStrVec() const {
    std::vector<std::string> moveNames;
    for (const json& move : moves) {
        moveNames.emplace_back(move["name"].get<std::string>());
    }
    return moveNames;
}

int Pokemon::longCalc(int dv, int ev, int level, int base) {
    return floor(0.01 * level * (2 * (base + dv) + floor(0.25 * ceil(pow(ev, 0.5)))));
}

std::vector<int> Pokemon::stats() {
    /*
    - TODO: incorporate stat stage changes in stats calculation 
    */

    // Compute the health DV
    DVs[0] = (DVs[1] % 2) * 8 + (DVs[2] % 2) * 4 + (DVs[3] % 2) * 2 + (DVs[4] % 2);

    // Calculate the stats
    std::vector<int> statsToReturn;
    statsToReturn.push_back(longCalc(DVs[0], EVs[0], level, baseStats[0]) + level + 10);
    for (int i = 1; i < 6; i++) {
        int initialStat = longCalc(DVs[i], EVs[i], level, baseStats[i]) + 5;
        if (statStages[i] > 0) {
            initialStat *= (2 + statStages[i]) / 2;
        }
        else if (statStages[i] < 0) {
            initialStat *= 2 / (2 - statStages[i]);
        }
        statsToReturn.push_back(initialStat);
    }

    return statsToReturn;
}

int Pokemon::confusionDamageDone(Pokemon* attacker, Pokemon* defender) {
    int power{40};
    int A{attacker->stats()[Stat::ATTACK]};
    int D{defender->stats()[Stat::DEFENSE]};
    int lvl{attacker->level};
    return floor(0.01 * (85 + Utils::randInt(0, 15)) * (2 + floor((0.4 * lvl + 2) * 0.8 * A/D)));
}

int Pokemon::damageDone(Pokemon* attacker, Pokemon* defender, int moveIdx) {
    int power = moves[moveIdx]["power"].get<int>();
    int attackerLevel = attacker->level;
    
    int attackerR, defenderR;
    if (moves[moveIdx]["type"].get<std::string>() == "physical") {
        attackerR = attacker->stats()[Stat::ATTACK];
        defenderR = defender->stats()[Stat::DEFENSE];
    }
    else {
        attackerR = attacker->stats()[Stat::SPECIAL_ATTACK];
        defenderR = defender->stats()[Stat::SPECIAL_DEFENSE];
    }

    return 2 + floor( (attackerR * power * 2 * (0.2 * attackerLevel + 1)) / (50 * defenderR));
}

void Pokemon::useMove(Pokemon* enemy, int moveIdx) {
    std::string moveType = moves[moveIdx]["move_type"].get<std::string>();
    std::string moveName = moves[moveIdx]["name"].get<std::string>();
    std::cout << nickname << " used " << moveName << "!" << std::endl;
    if (moveType == "physical" || moveType == "special") {
        useDmgingMove(enemy, moveIdx);
    }
    else if (moveType == "status") {
        useStatusMove(enemy, moveIdx);
    }
}

bool Pokemon::hasStatus(Status status) const {
    return (statuses.find(status) != statuses.end());
}

void Pokemon::useDmgingMove(Pokemon* enemy, int moveIdx) {
    int dmg = damageDone(this, enemy, moveIdx);
    int criticalStage = statStages[Stat::CRITICAL_HIT];
    int pCrit = criticalMultipliers[criticalStage] * stats()[Stat::SPEED];
    if (pCrit > 1.0f) pCrit = 1.0f;
    if (Utils::stdRand() < pCrit) {
        std::cout << "It was a critical hit!" << std::endl;
        dmg *= 2;
    }
    enemy->hp = std::max(0, enemy->hp - dmg);
    if (enemy->hp == 0) {
        enemy->statuses.insert(Status::FAINT);
    }
}

void Pokemon::useStatusMove(Pokemon* enemy, int moveIdx) {
    const json& move = moves[moveIdx];
    for (const json& effect : move["effects"].get<std::vector<json>>()) {
        if (effect.contains("probability")) {
            float p = effect["probability"].get<float>();
            if (Utils::stdRand() > p) continue;
        }

        Pokemon* target = effect["self"].get<bool>() ? this : enemy;

        if (effect["name"] == "stat-change") {
            Stat statToChange = strToStat(effect["stat"]);
            int change = effect["change"].get<int>();
            target->applyStatStageChange(statToChange, change);
        }
        else if (effect["name"] == "burn") {
            target->statuses.insert(Status::BURN);
        }
        else if (effect["name"] == "poison") {
            target->statuses.insert(Status::POISON);
        }
        else if (effect["name"] == "sleep") {
            target->statuses.insert(Status::SLEEP);
        }
        else if (effect["name"] == "paralyz") {
            target->statuses.insert(Status::PARALYZ);
        }
        else if (effect["name"] == "flinch") {
            enemy->statuses.insert(Status::FLINCH);
        }
        else if (effect["name"] == "confusion") {
            enemy->statuses.insert(Status::CONFUSION);
        }
        else if (effect["name"] == "infatuation") {
            enemy->statuses.insert(Status::INFATUATION);
        }
    }
}

void Pokemon::applyStatStageChange(Stat stat, int change) {
    if (stat == Stat::CRITICAL_HIT) {
        critHitStage += 1;
        if (critHitStage > 4) {
            critHitStage = 4;
        }
        return;
    }
    statStages[stat] += change;
    if (statStages[stat] > 6) {
        statStages[stat] = 6;
    }
    if (statStages[stat] < -6) {
        statStages[stat] = -6;
    }
}

void Pokemon::applyTurn(Pokemon* enemy, int moveIdx) {
    if (statuses.find(Status::BURN) != statuses.end()) {
        std::cout << nickname << " is hurt by the burn!" << std::endl;
        hp = std::max(0, hp - (int)(stats()[Stat::HP] * 0.125) );
        if (hp < 0) return;
    }
    if (statuses.find(Status::POISON) != statuses.end()) {
        std::cout << nickname << " is hurt by poison" << std::endl;
        hp = std::max(0, (int)floor(hp - stats()[Stat::HP] * 0.125));
        if (hp < 0) return;
    }
    if (statuses.find(Status::FROZEN) != statuses.end()) {
        std::cout << nickname << " is frozen solid and can't move!" << std::endl;
        if (Utils::stdRand() < 0.3) {
            std::cout << nickname << " was unfrozen!" << std::endl;
        }
        else {
            std::cout << nickname << " is frozen solid and can't move!" << std::endl;
            return;
        }
    }
    if (statuses.find(Status::SLEEP) != statuses.end()) {
        if (Utils::stdRand() < 0.3) {
            std::cout << nickname << " woke up!" << std::endl;
            statuses.erase(Status::SLEEP);
        }
        else {
            std::cout << nickname << " is fast asleep!" << std::endl;
            return;
        } 
    }
    if (statuses.find(Status::PARALYZ) != statuses.end()) {
        if (Utils::stdRand() < 0.3) {
            std::cout << nickname << " broke free from paralysis!" << std::endl;
            statuses.erase(Status::PARALYZ);
        }
        else if (Utils::stdRand() < 0.5) {
            std::cout << nickname << " is paralyzed and can't move!" << std::endl;
            return;
        } 
    }
    if (statuses.find(Status::FLINCH) != statuses.end()) {
        std::cout << nickname << " flinched!" << std::endl;
        statuses.erase(Status::FLINCH);
        return;
    }
    if (statuses.find(Status::CONFUSION) != statuses.end()) {
        if (Utils::stdRand() < 0.3) {
            std::cout << nickname << " broke free of their confusion!" << std::endl;
            statuses.erase(Status::CONFUSION);
        }
        else {
            std::cout << nickname << " is confused!" << std::endl;
            if (Utils::stdRand() < 0.5) {
                std::cout << nickname << " hurt themselves in their confusion!" << std::endl;
                hp = std::max(0, hp - confusionDamageDone(this, enemy));
                if (hp < 0) return;
            }
        }
    }
    if (statuses.find(Status::INFATUATION) != statuses.end()) {
        std::cout << nickname << " is infatuated!" << std::endl;
        return;
    }

    useMove(enemy, moveIdx);
}

Stat Pokemon::strToStat(std::string stat) {
    if (stat == "attack") return Stat::ATTACK;
    if (stat == "defense") return Stat::DEFENSE;
    if (stat == "special-attack") return Stat::SPECIAL_ATTACK;
    if (stat == "special-defense") return Stat::SPECIAL_DEFENSE;
    if (stat == "speed") return Stat::SPEED;
    if (stat == "hp") return Stat::HP;
    return Stat::ATTACK; // default null value;
}

std::string Pokemon::statToStr(Stat stat) {
    if (stat == Stat::ATTACK) return "attack";
    if (stat == Stat::DEFENSE) return "defense";
    if (stat == Stat::SPECIAL_ATTACK) return "special-attack";
    if (stat == Stat::SPECIAL_DEFENSE) return "special-defense";
    if (stat == Stat::SPEED) return "speed";
    if (stat == Stat::HP) return "hp";
    return ""; // default null value;
}