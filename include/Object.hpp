#pragma once

#include "SpriteAnimation.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Object : public SpriteAnimation {
public:
    std::string onInteractFuncName;
    json data;
    Object(TilesetName tilesetName, int x, int y, float layer, float frameDuration, bool hasCollision, std::vector<int> ids, json data);
    Object(SpriteAnimation& tile, json data);
};