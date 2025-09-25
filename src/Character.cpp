#pragma once
#include "Character.hpp"
#include "SpriteAnimation.hpp"
#include "CharacterAnimationState.hpp"
#include "TilesetName.hpp"
#include "Direction.hpp"
#include "constants.hpp"
#include "Screen.hpp"
#include "Game.hpp"

#include <fstream>
#include <iostream>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Character::Character(std::string characterName, int x, int y) :
    x(x),
    y(y),
    tileX(x / CScreen::tileSize),
    tileY(y / CScreen::tileSize),
    moving(false),
    direction(DOWN),
    movingStartTime(0.0f),
    tileTime(CCharacter::characterTileTime)
{  
    spriteAnimations.resize(NUM_CHARACTER_ANIMATIONS);
    std::ifstream file(CJson::characterPath);
    json j;
    file >> j;

    json characterData = j[characterName];
    for (int i = 0; i < NUM_CHARACTER_ANIMATIONS; i++) {
        spriteAnimations[i] = SpriteAnimation(CHARACTER, x, y, CCharacter::characterLayer, CCharacter::characterFrameDuration, true, characterData[i].get<std::vector<int>>());
    }
}

std::pair<int,int> Character::getTargetTilePos() const {
    if (direction == UP) {
        return {tileX*CScreen::tileSize, (tileY - 1)*CScreen::tileSize};
    }
    else if (direction == RIGHT) {
        return {(tileX+1)*CScreen::tileSize, tileY*CScreen::tileSize};
    }
    else if (direction == DOWN) {
        return {tileX*CScreen::tileSize, (tileY + 1)*CScreen::tileSize};
    }  
    else {
        return {(tileX-1)*CScreen::tileSize, tileY*CScreen::tileSize};
    }
}

bool Character::move(Direction dir) {
    if (moving) return false;

    // Check : are we about to move into a collision tile?
    // Check map tiles and NPCs
    direction = dir;
    std::pair<int, int> targetTilePos = getTargetTilePos();  
    int targetTileX = targetTilePos.first / CScreen::tileSize;
    int targetTileY = targetTilePos.second / CScreen::tileSize;
    Map& map = Game::getInstance().map;
    for (int layer = 0; layer < map.numLayers; layer++) {
        if (map.isTileCollision(getTargetTilePos().first, getTargetTilePos().second, layer)) {
            return false;
        } 
    }
    // Check collision with Player, who is NOT on the map
    Player& player = Game::getInstance().player;
    if (player.tileX == targetTileX && player.tileY == targetTileY) return false;
    
    movingStartTime = glfwGetTime();
    moving = true;
    tileX = getTargetTilePos().first / CScreen::tileSize;
    tileY = getTargetTilePos().second / CScreen::tileSize;
    return true;
}

void Character::push() {
    Drawer& drawer = Game::getInstance().drawer;
    Player& player = Game::getInstance().player;

    // Update the character position if they're moving
    if (moving) {
        float alpha = (glfwGetTime() - movingStartTime) / tileTime;
        if (alpha >= 1) {
            alpha = 1.0f;
        }
        if (direction == UP) {
            float y1 = (tileY + 1) * CScreen::tileSize;
            float y2 = tileY * CScreen::tileSize;
            y = static_cast<int>(y1 + (y2 - y1) * alpha);
        }
        else if (direction == RIGHT) {
            float x1 = (tileX - 1) * CScreen::tileSize;
            float x2 = tileX * CScreen::tileSize;
            x = static_cast<int>(x1 + (x2 - x1) * alpha);
        }
        else if (direction == DOWN) {
            float y1 = (tileY - 1) * CScreen::tileSize;
            float y2 = tileY * CScreen::tileSize;
            y = static_cast<int>(y1 + (y2 - y1) * alpha);
        }
        else if (direction == LEFT) {
            float x1 = (tileX + 1) * CScreen::tileSize;
            float x2 = tileX * CScreen::tileSize;
            x = static_cast<int>(x1 + (x2 - x1) * alpha);
        }
        if (alpha >= 1.0f) {
            spriteAnimations[animationState()].x = x;
            spriteAnimations[animationState()].y = y;
            
            drawer.addSprite(&spriteAnimations[animationState()], -1, -1);
            moving = false;
            return;
        }
    }
    spriteAnimations[animationState()].x = x;
    spriteAnimations[animationState()].y = y;
    drawer.addSprite(&spriteAnimations[animationState()], -1, -1);
}

CharacterAnimationState Character::animationState() {
    if (moving) {
        if (direction == UP) {
            return WALK_UP;
        }
        else if (direction == RIGHT) {
            return WALK_RIGHT;
        }
        else if (direction == DOWN) {
            return WALK_DOWN;
        }
        else {
            return WALK_LEFT;
        }
    }
    else {
        if (direction == UP) {
            return STAND_UP;
        }
        else if (direction == RIGHT) {
            return STAND_RIGHT;
        }
        else if (direction == DOWN) {
            return STAND_DOWN;
        }
        else {
            return STAND_LEFT;
        }
    }
}