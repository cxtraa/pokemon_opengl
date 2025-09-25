#pragma once
#include "Character.hpp"
#include "Direction.hpp"
#include "WarpPoint.hpp"
#include "Pack.hpp"

class Player : public Character {
public:
    Player(int x, int y);
    void handleInput();
    void update();
    void checkMapSwitch();
    void checkGrass();

    bool awaitingMoveFinish;
    WarpPoint bufferedWarpPoint;
    Pack pack;
};