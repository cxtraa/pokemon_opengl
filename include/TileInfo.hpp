#pragma once

#include <vector>

struct TileInfo {
    bool hasAnimation = false;
    bool hasCollision = false;
    float frameDuration = 0.0f;
    std::vector<int> animationIds;
};