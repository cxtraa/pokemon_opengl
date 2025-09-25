#pragma once

#include "TilesetName.hpp"
#include <string>
#include <vector>

class SpriteAnimation {
public:
    SpriteAnimation(TilesetName tilesetName, int x, int y, float layer, float frameDuration, bool hasCollision, std::vector<int> ids);
    SpriteAnimation();
    virtual ~SpriteAnimation() = default; // polymorphism

    float lastUpdateTime;
    TilesetName tilesetName;
    int x, y;
    float layer;
    int idIndex;
    int numFrames;
    float frameDuration;
    std::vector<int> ids;
    bool hasCollision;

    void draw(int X, int Y);
    void nextFrame();
    bool hasAnimation() const;
};