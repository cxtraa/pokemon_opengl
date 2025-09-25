
#include "Drawer.hpp"
#include "SpriteAnimation.hpp"
#include "DrawerEntry.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

Drawer::Drawer() {}

void Drawer::addSprite(SpriteAnimation* spritePtr, int x, int y) {
    sprites.push_back(DrawerEntry(spritePtr, x, y));
}

void Drawer::sortSprites() {
    std::sort(
        sprites.begin(),
        sprites.end(),
        [](const DrawerEntry& a, const DrawerEntry& b) {
            return a.sprite->layer < b.sprite->layer;
        }
    );
}

void Drawer::draw(int cameraX, int cameraY) {
    for (int i = 0; i < sprites.size(); i++) {
        int x = sprites[i].x;
        int y = sprites[i].y;
        if (x == -1 && y == -1) {
            // render according to camera instead
            sprites[i].sprite->draw(sprites[i].sprite->x - cameraX, sprites[i].sprite->y - cameraY);
        }
        else {
            sprites[i].sprite->draw(x, y);
        }
    }
}

void Drawer::clear() {
    sprites.clear();
}