
#include "Game.hpp"
#include "constants.hpp"
#include "Player.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

Game::Game() :
    state(),
    buttonHandler(CInput::cooldownTime),
    drawer(),
    tilesetManager(),
    dialogueManager(),
    menuUI(),
    map("./tiled_project/map_1", &tilesetManager, &state),
    player(1 * CScreen::tileSize, 2 * CScreen::tileSize)
{}

Game& Game::getInstance() {
    static Game game;
    return game;
}

void Game::draw() {
    drawer.clear();
    player.update();
    player.push();
    map.push();
    dialogueManager.push();
    menuUI.push();
    player.pack.push();
    drawer.sortSprites();
    
    // Center the camera on the player
    int offset_x = CScreen::tileSize * (CScreen::numTilesX - 1) / 2;
    int offset_y = CScreen::tileSize * (CScreen::numTilesY - 1) / 2;
    int cameraX = player.x - offset_x;
    int cameraY = player.y - offset_y;
    drawer.draw(cameraX, cameraY);
}

void Game::switchMap(WarpPoint& warpPoint) {
    fs::path mapsDirectoryPath(CJson::mapsDir);
    fs::path newMapDir = mapsDirectoryPath / (warpPoint.mapToWarpTo);
    std::cout << newMapDir.string() << std::endl;
    map = std::move(Map(newMapDir.string(), &tilesetManager, &state));
    player.x = warpPoint.targetX;
    player.y = warpPoint.targetY;
    player.tileX = warpPoint.targetX / CScreen::tileSize;
    player.tileY = warpPoint.targetY / CScreen::tileSize;
    player.direction = warpPoint.targetDir;
}