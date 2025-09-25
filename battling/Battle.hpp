#pragma once
#include "Trainer.hpp"

class Battle {
public:
    Battle(Trainer* player, Trainer* enemy);
    Trainer* player;
    Trainer* enemy;
    void battle();
    void doPlayerTurn();
    void doEnemyTurn();
    void playerSwitchPokemon();
    bool checkBattleOver();
};  