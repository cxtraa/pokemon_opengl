
#pragma once

#include "Direction.hpp"
#include "SpriteAnimation.hpp"
#include "Object.hpp"
#include "CharacterAnimationState.hpp"

class Character {
public:
    std::vector<SpriteAnimation> spriteAnimations;
    int x, y, tileX, tileY;
    bool moving;
    Direction direction;
    float movingStartTime;
    float tileTime;
    
    std::pair<int, int> getTargetTilePos() const;
    bool move(Direction direction);
    CharacterAnimationState animationState();
    Character(std::string characterName, int x, int y);
    void push();
};