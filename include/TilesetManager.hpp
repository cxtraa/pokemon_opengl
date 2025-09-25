#pragma once
#include "Tileset.hpp"
#include <vector>

class TilesetManager {
public:
    std::vector<Tileset> data; // indices are TilesetName
    TilesetManager();
    ~TilesetManager();
};