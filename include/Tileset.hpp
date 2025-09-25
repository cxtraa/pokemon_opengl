#pragma once
#include <string>
#include <unordered_map>
#include "TileInfo.hpp"

class Tileset {
public:
    std::string path;
    int tileWidth;
    int tileHeight;
    int channels;
    int width;
    int height;

    Tileset(std::string tilesetName, int tileWidth, int tileHeight);
    std::unordered_map<int, TileInfo> tileToInfo;

    unsigned char* img;
};