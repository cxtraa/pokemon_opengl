#pragma once

#include "SpriteAnimation.hpp"
#include "TileInfo.hpp"
#include "Drawer.hpp"
#include "WarpPoint.hpp"
#include "TilesetManager.hpp"
#include "TilePosition.hpp"
#include "GameState.hpp"
#include "NPC.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Map {
public:
    int startX, startY;
    std::string name;
    int mapTilesX, mapTilesY, numLayers;
    TilesetManager* tilesetManager;
    GameState* state;
    Map(std::string mapDir, TilesetManager* tilesetManager, GameState* state);
    std::vector<WarpPoint> warpPoints;
    std::unordered_map<TilePosition, std::unique_ptr<SpriteAnimation>> mapTiles;
    std::vector<std::unique_ptr<NPC>> NPCs;
    bool isTileCollision(int tileX, int tileY, int layerNum);
    void push();
    bool handleInteract(int x, int y, int layer);
    void processWarpPointsLayer(json& layer, int layerNum);
    void processInteractablesLayer(json& layer, int layerNum);
    void processTileLayer(json& layer, int layerNum);
    void processNpcData(std::string mapJsonPathStr);
    void removeObject(int x, int y, int layer);
    bool isObject(int x, int y, int layer);
    int getTileID(int x, int y, int layerNum);
    std::unique_ptr<SpriteAnimation> constructTile(int id, int x, int y, int layerNum);
    std::unique_ptr<SpriteAnimation> constructObject(int id, int x, int y, int layerNum, json data);
};