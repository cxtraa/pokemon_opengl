#pragma once

#include "SpriteAnimation.hpp"

struct DrawerEntry {
    SpriteAnimation* sprite;
    int x, y;
    DrawerEntry(SpriteAnimation* sprite, int x, int y) : sprite(sprite), x(x), y(y) {}
};