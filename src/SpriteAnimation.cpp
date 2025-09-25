
#include "SpriteAnimation.hpp"
#include "stb_image.h"
#include "constants.hpp"
#include "Screen.hpp"
#include "Tileset.hpp"
#include "TilesetName.hpp"
#include "Game.hpp"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>

SpriteAnimation::SpriteAnimation(TilesetName tilesetName, int x, int y, float layer, float frameDuration, bool hasCollision, std::vector<int> ids) :
    tilesetName(tilesetName),
    layer(layer),
    frameDuration(frameDuration),
    x(x), // global x, y coordinates
    y(y),
    ids(ids),
    hasCollision(hasCollision),
    idIndex(0),
    numFrames(ids.size()),
    lastUpdateTime(0.0f)
{}

SpriteAnimation::SpriteAnimation() {}

void SpriteAnimation::draw(int X, int Y) {
    if (hasAnimation() && glfwGetTime() - lastUpdateTime > frameDuration) {
        lastUpdateTime = glfwGetTime();
        nextFrame();
    }

    Game& game = Game::getInstance();
    Screen& screen = Screen::getInstance();
    TilesetManager& tilesetManager = game.tilesetManager;
    Tileset& tileset = tilesetManager.data[tilesetName];
    unsigned char* tilesetImage = tileset.img;
    int tilesetWidth = tileset.width;
    int tilesetHeight = tileset.height;
    int tilesetChannels = tileset.channels;
    int tileHeight = tileset.tileHeight;
    int tileWidth = tileset.tileWidth;
    
    for (int i = 0; i < tileHeight; i++) {
        for (int j = 0; j < tileWidth; j++) {
            int numTilesX = tilesetWidth / tileWidth;
            int numTilesY = tilesetHeight / tileHeight;
            int id = ids[idIndex];
  
            int tileX = id % numTilesX;
            int tileY = (id / numTilesX);
            int idx = tilesetChannels * ((tileY * tileHeight + i) * tilesetWidth + (tileX * tileWidth + j));

            unsigned char r = tilesetImage[idx + 0];
            unsigned char g = tilesetImage[idx + 1];
            unsigned char b = tilesetImage[idx + 2];
            unsigned char a = tilesetImage[idx + 3];
            
            screen.drawPixel(X + j, Y + i, r, g, b, a);
        }
    }
}

void SpriteAnimation::nextFrame() {
    idIndex = (idIndex + 1) % numFrames;
}

bool SpriteAnimation::hasAnimation() const {
    return (numFrames > 1);
}