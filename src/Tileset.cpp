
#include "Tileset.hpp"
#include "constants.hpp"
#include "stb_image.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <iostream>

using json = nlohmann::json;
namespace fs = std::filesystem;

Tileset::Tileset(std::string tilesetName, int tileWidth, int tileHeight) : tileWidth(tileWidth), tileHeight(tileHeight)
{
    fs::path tilesetsDir(CJson::mapsDir);
    fs::path jsonPath = tilesetsDir / (tilesetName + ".json");
    fs::path tilesetPath = tilesetsDir / (tilesetName + ".png");
    std::string jsonPathStr = jsonPath.string();
    std::string tilesetPathStr = tilesetPath.string();

    // do stbi load and init img, channels, width, height
    img = stbi_load(tilesetPathStr.c_str(), &width, &height, &channels, 0);

    std::ifstream file(jsonPathStr);
    if (!file) return;
    json j;
    file >> j;
    const auto& tiles = j["tiles"].get<std::vector<json>>();
    for (const auto& tile : tiles) {
        TileInfo tileInfo;
        int tileid = 0;
        if (tile.contains("objectgroup")) {
            tileInfo.hasCollision = true;
            tileid = tile["id"].get<int>();
        }
        if (tile.contains("animation")) {
            tileInfo.hasAnimation = true;
            const auto& animation = tile["animation"];
            tileid = animation[0]["tileid"];
            float frameDuration = animation[0]["duration"];
            tileInfo.frameDuration = frameDuration / 1000;
            for (const auto& frame : animation) {
                tileInfo.animationIds.push_back(frame["tileid"].get<int>());
            }
        }
        tileToInfo[tileid] = tileInfo;
    }
}