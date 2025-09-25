#pragma once

#include "SpriteAnimation.hpp"
#include "Player.hpp"
#include "DrawerEntry.hpp"
#include <memory>
#include <vector>
#include <tuple>

class Drawer {
public:
    std::vector<DrawerEntry> sprites;

    Drawer();

    void addSprite(SpriteAnimation* spritePtr, int x, int y); // add a sprite to the vector
    void sortSprites(); // sort the sprites by their layer
    void draw(int cameraX, int cameraY);
    void clear(); // clear all sprites from the vector
};