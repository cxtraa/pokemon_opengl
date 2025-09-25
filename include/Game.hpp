
#pragma once
#include "Player.hpp"
#include "Drawer.hpp"
#include "Map.hpp"
#include "ButtonHandler.hpp"
#include "TilesetManager.hpp"
#include "WarpPoint.hpp"
#include "DialogueManager.hpp"
#include "GameState.hpp"
#include "MenuUI.hpp"

class Game {
protected:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
public:
    GameState state;
    ButtonHandler buttonHandler;
    Drawer drawer;
    TilesetManager tilesetManager;
    DialogueManager dialogueManager;
    MenuUI menuUI;
    Map map;
    Player player;
    
    static Game& getInstance();

    void draw();
    void switchMap(WarpPoint& warpPoint);
};