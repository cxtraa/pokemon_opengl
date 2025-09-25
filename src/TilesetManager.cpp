
#include "TilesetManager.hpp"
#include "constants.hpp"
#include "stb_image.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

TilesetManager::TilesetManager() {
    json j;
    std::ifstream file(CJson::tilesetPath);
    file >> j;
    std::vector<json> tilesets = j["tilesets"].get<std::vector<json>>();
    for (const json& t : tilesets) {
        std::string path = t["path"].get<std::string>();
        fs::path tilesetPath(path);
        std::string tilesetName = tilesetPath.stem();
        int tileWidth = t["tileWidth"].get<int>();
        int tileHeight = t["tileHeight"].get<int>();
        Tileset tileset(tilesetName, tileWidth, tileHeight);
        data.push_back(tileset);
    }
}

TilesetManager::~TilesetManager() {
    for (int i = 0; i < data.size(); i++) {
        stbi_image_free(data[i].img);
    }
}