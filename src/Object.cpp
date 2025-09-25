
#include "Object.hpp"
#include "Map.hpp"
#include "Utils.hpp"
#include <iostream>

Object::Object(TilesetName tilesetName, int x, int y, float layer, float frameDuration, bool hasCollision, std::vector<int> ids, json data) : 
    SpriteAnimation(tilesetName, x, y, layer, frameDuration, hasCollision, ids),
    data(data)
{
    onInteractFuncName = Utils::getProperty<std::string>(data, "onInteractFuncName");
}

Object::Object(SpriteAnimation& tile, json data) :
    SpriteAnimation(tile),
    data(data)
{
    onInteractFuncName = Utils::getProperty<std::string>(data, "onInteractFuncName");
}