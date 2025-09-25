
#include "Map.hpp"
#include "SpriteAnimation.hpp"
#include "constants.hpp"
#include "Drawer.hpp"
#include "Game.hpp"
#include "WarpPoint.hpp"
#include "TilePosition.hpp"
#include "Utils.hpp"
#include "ObjectFunctions.hpp"
#include "NPCFunctions.hpp"
#include "Object.hpp"
#include "NPC.hpp"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <memory>

using json = nlohmann::json;
namespace fs = std::filesystem;

Map::Map(std::string mapDir, TilesetManager* tilesetManager, GameState* state) : tilesetManager(tilesetManager), state(state) {
    // Get the path to the map json and the map npcs json
    fs::path mapDirPath(mapDir);
    std::string mapName = mapDirPath.filename().string();
    fs::path npcDataJsonPath = mapDirPath / (mapName + "_npc_data.json");
    fs::path mapJsonPath = mapDirPath / (mapName + ".json");
    std::string npcDataJsonPathStr = npcDataJsonPath.string();
    std::string mapJsonPathStr = mapJsonPath.string();

    // Handle the NPCs
    processNpcData(npcDataJsonPathStr);

    std::ifstream file(mapJsonPathStr);
    if (!file) {
        throw std::runtime_error("Map: the mapJsonPath was invalid.");
    }
    fs::path filePath(mapJsonPathStr);
    name = filePath.stem().string();
    json j;
    file >> j;
    mapTilesY = j["height"].get<int>();
    mapTilesX = j["width"].get<int>();
    int totalMapTiles = mapTilesX*mapTilesY;

    // Initialise the vector of tiles
    std::vector<json> layers = j["layers"].get<std::vector<json>>();
    numLayers = layers.size();
    for (int layerNum = 0; layerNum < numLayers; layerNum++) {
        std::string className = layers[layerNum]["class"].get<std::string>();
        if (className == "warpPoints") {
            processWarpPointsLayer(layers[layerNum], layerNum);
        }
        else if (className == "interactableLayer") {
            processInteractablesLayer(layers[layerNum], layerNum);
        }
        else if (className == "tileLayer") {
            processTileLayer(layers[layerNum], layerNum);
        }
    }
}

std::unique_ptr<SpriteAnimation> Map::constructTile(int id, int x, int y, int layerNum) {
    // Construct tile, checking if it has a collision / animation
    std::unique_ptr<SpriteAnimation> tile;
    std::vector<int> ids{id};
    std::unordered_map<int, TileInfo>& tileToInfo = tilesetManager->data[MAP].tileToInfo;
    if (tileToInfo.find(id) != tileToInfo.end()) {
        TileInfo tileInfo = tileToInfo[id];
        if (tileInfo.hasAnimation) {
            ids = tileInfo.animationIds;
        }
        tile = std::make_unique<SpriteAnimation>(MAP, x, y, static_cast<float>(layerNum), tileInfo.frameDuration, tileInfo.hasCollision, ids);
    }
    else {
        tile = std::make_unique<SpriteAnimation>(MAP, x, y, static_cast<float>(layerNum), 0.0f, false, ids);
    }
    return tile;
}

std::unique_ptr<SpriteAnimation> Map::constructObject(int id, int x, int y, int layerNum, json data) {
    std::unique_ptr<SpriteAnimation> tile = constructTile(id, x, y, layerNum);
    std::unique_ptr<SpriteAnimation> object = std::make_unique<Object>(*tile, data);
    return object;
}

int Map::getTileID(int x, int y, int layerNum) {
    TilePosition pos(x, y, layerNum);
    if (mapTiles.find(pos) != mapTiles.end()) {
        return mapTiles[pos]->ids[0];
    }
    return -1;
}

void Map::processTileLayer(json& layer, int layerNum) {
    std::vector<int> tileIDs = layer["data"].get<std::vector<int>>();
    for (int idx = 0; idx < tileIDs.size(); idx++) {
        int id = tileIDs[idx] - 1;
        if (id == -1) continue;
        int x = (idx % mapTilesX) * CScreen::tileSize;
        int y = (idx / mapTilesX) * CScreen::tileSize;
        std::unique_ptr<SpriteAnimation> tile = constructTile(id, x, y, layerNum);
        TilePosition pos(x, y, layerNum);
        mapTiles[pos] = std::move(tile);
    }
}

void Map::processWarpPointsLayer(json& layer, int layerNum) {
    std::vector<json> objects = layer["objects"].get<std::vector<json>>();
    for (const json& object : objects) {
        std::string type = object["type"].get<std::string>();
        int w = object["width"].get<int>();
        int h = object["height"].get<int>();
        int x = object["x"].get<int>();
        int y = object["y"].get<int>();
        if (type == "warp") {
            std::string mapToWarpTo;
            int targetX, targetY;
            Direction targetDir;
            for (const json& property : object["properties"].get<std::vector<json>>()) {
                std::string name = property["name"].get<std::string>();
                if (name == "mapToWarpTo") {
                    mapToWarpTo = property["value"].get<std::string>();
                }
                else if (name == "targetX") {
                    targetX = property["value"].get<int>();
                }
                else if (name == "targetY") {
                    targetY = property["value"].get<int>();
                }
                else if (name == "targetDir") {
                    targetDir = property["value"].get<Direction>();
                }
            }
            WarpPoint warpPoint(x, y, x + w, y + h, targetX, targetY, targetDir, mapToWarpTo);
            warpPoints.push_back(warpPoint);
        }
        else if (type == "start") {
            startX = x;
            startY = y;
        }
    }

    std::cout << "Map: successfully processed warp points." << std::endl;
}

void Map::processInteractablesLayer(json& layer, int layerNum) {
    // Process a layer of class interactables
    std::vector<json> objects = layer["objects"].get<std::vector<json>>();
    for (json& object : objects) {
        int tileId = object["gid"].get<int>() - 1;
        int objectId = object["id"].get<int>();
        if (state->idOfCollectedItems.find(objectId) != state->idOfCollectedItems.end()) {
            continue;
        }
        int x = object["x"].get<int>();
        int y = object["y"].get<int>();
        std::unique_ptr<SpriteAnimation> tileObject = constructObject(tileId, x, y, layerNum, object);
        TilePosition pos(x, y, layerNum);
        mapTiles[pos] = std::move(tileObject);
    }
    
    std::cout << "Map: successfully processed interactables layer." << std::endl;
}

void Map::processNpcData(std::string mapJsonPathStr) {
    // Open the JSON
    std::ifstream file(mapJsonPathStr);
    json j;
    file >> j;

    // Get the data
    const std::vector<json>& npcs = j["npcs"].get<std::vector<json>>();

    // Iterate through the NPCs
    for (const json& npc : npcs) {
        // Extract relevant info
        std::string id = npc["id"].get<std::string>();
        int startX = npc["startX"].get<int>();
        int startY = npc["startY"].get<int>();
        std::string characterName = npc["characterName"].get<std::string>();
        std::vector<int> movements = npc["movements"].get<std::vector<int>>();
        std::string onInteractFuncName = npc["onInteractFuncName"].get<std::string>();
        std::string dialogue = npc["dialogue"].get<std::string>();

        std::unique_ptr<NPC> npcPtr = std::make_unique<NPC>(characterName, startX, startY, onInteractFuncName, movements, dialogue);
        NPCs.emplace_back(std::move(npcPtr));
    }

    std::cout << "NPC: Successfully processed NPCs" << std::endl;
}

void Map::push() {
    // Perform updates here
    for (int i = 0; i < NPCs.size(); i++) {
        NPCs[i]->updateMove();
        NPCs[i]->push();
    }

    Game& game = Game::getInstance();
    Player& player = game.player;
    Drawer& drawer = game.drawer;
    for (auto& [pos, tile] : mapTiles) {
        int x = tile->x;
        int y = tile->y;
        drawer.addSprite(tile.get(), -1, -1);
    }
}

bool Map::isObject(int x, int y, int layer) {
    TilePosition pos(x, y, layer);
    return mapTiles.find(pos) != mapTiles.end() && dynamic_cast<Object*>(mapTiles[pos].get());
}

bool Map::handleInteract(int x, int y, int layer) {
    // Check for NPC first
    int tileX = x / CScreen::tileSize;
    int tileY = y / CScreen::tileSize;
    for (int i = 0; i < NPCs.size(); i++) {
        if (tileX == NPCs[i]->tileX && tileY == NPCs[i]->tileY) {
            NPCFunctions::invoke(NPCs[i].get(), NPCs[i]->onInteractFuncName);
            return true;
        }
    }
    TilePosition pos(x, y, layer);
    if (!isObject(x, y, layer)) return false;
    Object* object = dynamic_cast<Object*>(mapTiles[pos].get());
    std::string onInteractFuncName = Utils::getProperty<std::string>(object->data, "onInteractFuncName");
    ObjectFunctions::invoke(object, onInteractFuncName);
    return true;
}


void Map::removeObject(int x, int y, int layer) {
    TilePosition pos(x, y, layer);
    // Does tile exist, and is it an Object?
    auto it = mapTiles.find(pos);
    if (it != mapTiles.end() && dynamic_cast<Object*>(mapTiles[pos].get())) {
        mapTiles.erase(it);
    }
}

bool Map::isTileCollision(int x, int y, int layerNum) {
    if (x < 0 || x >= mapTilesX*CScreen::tileSize || y < 0 || y >= mapTilesY*CScreen::tileSize) return true;
    TilePosition pos = TilePosition(x, y, layerNum);
    if (mapTiles.find(pos) != mapTiles.end() && mapTiles[pos]->hasCollision) return true;
    // Now check if there's an NPC there
    int tileX = x / CScreen::tileSize;
    int tileY = y / CScreen::tileSize;
    for (int i = 0; i < NPCs.size(); i++) {
        int targetTileX = NPCs[i]->x / CScreen::tileSize;
        int targetTileY = NPCs[i]->y / CScreen::tileSize;
        if (tileX == targetTileX && tileY == targetTileY) return true;
    }
    return false;
}