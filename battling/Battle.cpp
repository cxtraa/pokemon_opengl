
#include "Battle.hpp"
#include "Utils.hpp"

Battle::Battle(Trainer* player, Trainer* enemy) : player(player), enemy(enemy) {}

void Battle::battle() {
    while (true) {
        doPlayerTurn();
        if (checkBattleOver()) break;
        doEnemyTurn();
        if (checkBattleOver()) break;
    }
}

void Battle::doPlayerTurn() {
    Pokemon* playerMon = player->currMon();
    Pokemon* enemyMon = enemy->currMon();

    std::cout << std::endl;
    std::cout << "PLAYER" << std::endl;
    std::cout << "Current Pokemon: " << player->currMon()->nickname << std::endl;
    std::cout << "HP: " << player->currMon()->hp << std::endl;

    int moveIdx = -1;
    for (int i = 0; i < playerMon->moves.size(); i++) {
        const json& move = playerMon->moves[i];
        std::cout << i << " - " << move["name"] << std::endl;
    }
    std::cout << playerMon->moves.size() << " - Switch pokemon" << std::endl;
    while (moveIdx < 0 || moveIdx > playerMon->moves.size()) {
        std::cout << "Type in the number of the move to use: ";
        std::cin >> moveIdx;
    }
    
    if (moveIdx == playerMon->moves.size()) {
        playerSwitchPokemon();
    }
    else {
        playerMon->applyTurn(enemyMon, moveIdx);
    }
}

void Battle::doEnemyTurn() {
    Pokemon* playerMon = player->currMon();
    Pokemon* enemyMon = enemy->currMon();

    std::cout << std::endl;
    std::cout << "ENEMY " << enemy->name << std::endl;
    std::cout << "Current Pokemon: " << enemy->currMon()->nickname << std::endl;
    std::cout << "HP: " << enemy->currMon()->hp << std::endl;

    int moveIdx = Utils::randInt(0, 3);
    enemyMon->applyTurn(playerMon, moveIdx);
}

void Battle::playerSwitchPokemon() {
    std::cout << "Please choose the next Pokemon to use." << std::endl;
    std::vector<int> faintedMonIdxs;
    for (int i = 0; i < player->pokemon.size(); i++) {
        std::cout << i << "- " << player->pokemon[i]->nickname;
        if (player->pokemon[i]->hasStatus(Status::FAINT)) {
            std::cout << " (FAINT)";
            faintedMonIdxs.push_back(i);
        }
        if (player->pokemon[i]->id == player->currMon()->id) {
            std::cout << " (CURR)";
        }
        std::cout << std::endl;
    }
    int pokemonIdx = 0;
    std::cin >> pokemonIdx;
    auto faint = [&](int idx) -> bool {
        if (std::find(faintedMonIdxs.begin(), faintedMonIdxs.end(), pokemonIdx) != faintedMonIdxs.end()) {
            std::cout << "The selected Pokemon has already fainted. Please try again." << std::endl;
            return true;
        }
        return false;
    };
    auto curr = [&](int idx) -> bool {
        if (player->pokemon[idx]->id == player->currMon()->id) {
            std::cout << "This pokemon is already out!" << std::endl;
            return true;
        }
        return false;
    };
    auto outOfBounds = [&](int idx) -> bool {
        if (idx < 0 || idx > player->pokemon.size()) {
            std::cout << "The index selected is out of bounds." << std::endl;
            return true;
        }
        return false;
    };
    while (outOfBounds(pokemonIdx) || faint(pokemonIdx) || curr(pokemonIdx)) {
        std::cin >> pokemonIdx;
    }
    player->currMonIdx = pokemonIdx;
    std::cout << player->name << " sent out " << player->currMon()->nickname << std::endl;
}

bool Battle::checkBattleOver() {
    if (player->isDead()) {
        std::cout << "Player has run out of Pokemon to use. Enemy wins." << std::endl;
        return true;
    }
    else if (enemy->isDead()) {
        std::cout << "Enemy was defeated!" << std::endl;
        return true;
    }

    if (player->currMon()->hasStatus(Status::FAINT)) {
        std::cout << "Your " << player->currMon()->nickname << " fainted." << std::endl;
        playerSwitchPokemon();
    }

    if (enemy->currMon()->hasStatus(Status::FAINT)) {
        std::cout << "Enemy " << enemy->currMon()->nickname << " fainted." << std::endl;
        std::vector<int> aliveMonIdxs;
        for (int i = 0; i < enemy->pokemon.size(); i++) {
            if (!enemy->pokemon[i]->hasStatus(Status::FAINT)) {
                aliveMonIdxs.push_back(i);
            }
        }
        int enemyNewMonIdx = aliveMonIdxs[Utils::randInt(0, aliveMonIdxs.size() - 1)];

        enemy->currMonIdx = enemyNewMonIdx;
        std::cout << enemy->name << " sent out " << enemy->currMon()->nickname << std::endl;
    }

    return false;
}