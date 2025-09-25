
#include "Player.hpp"
#include "Character.hpp"
#include "Screen.hpp"
#include "DialogueManager.hpp"
#include "constants.hpp"
#include "Game.hpp"
#include "Utils.hpp"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Player::Player(int x, int y) :
    Character("playerMale", x, y)
{}

void Player::checkGrass() {
    Map& map = Game::getInstance().map;
    int id = map.getTileID(x, y, 0);
    // std::cout << id << std::endl;
    if (id == 17) {
        std::cout << "On grass" << std::endl;
        float r = Utils::randFloat();
        if (r < CPokemonEncounter::pEncounter) {
            Game::getInstance().dialogueManager.display("Wild pokemon found!!");
        }
    }
}

void Player::handleInput() {
    Screen& screen = Screen::getInstance();
    GLFWwindow* window = screen.getWindow();
    Game& game = Game::getInstance();
    DialogueManager& dialogueManager = game.dialogueManager;
    MenuUI& menuUI = game.menuUI;
    Map& map = game.map;
    ButtonHandler& buttonHandler = game.buttonHandler;

    if (dialogueManager.displayingText || dialogueManager.awaitingUserContinue) return;
    if (menuUI.visible) return;
    if (pack.visible) return;
    
    // MOVEMENT CONTROLS
    if (!moving) {
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            tileTime = CPlayer::playerRunTileTime;
        }
        else {
            tileTime = CPlayer::playerWalkTileTime;
        }
        
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            move(UP);
            checkGrass();
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            move(RIGHT);
            checkGrass();
        }
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            move(DOWN);
            checkGrass();
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            move(LEFT);
            checkGrass();
        }
    }


    // INTERACT WITH OBJECTS
    if (!moving) {
        if (buttonHandler.isPressed(GLFW_KEY_Z)) {
            // Iterate through layers
            for (int layer = CMap::maxLayers - 1; layer >= 0; layer--) {
                if (map.handleInteract(getTargetTilePos().first, getTargetTilePos().second, layer)) break;
            }
        }
    }
}

void Player::update() {
    handleInput();
    checkMapSwitch();
    if (awaitingMoveFinish && !moving) {
        // Perform the requested map switch
        awaitingMoveFinish = false;
        Game& game = Game::getInstance();
        game.switchMap(bufferedWarpPoint);
    }
}

void Player::checkMapSwitch() {
    int xMid = x + 8;
    int yMid = y + 8;
    Game& game = Game::getInstance();
    Map& map = game.map;
    for (const WarpPoint& warpPoint : map.warpPoints) {
        if (warpPoint.x1 < xMid && xMid < warpPoint.x2 && warpPoint.y1 < yMid && yMid < warpPoint.y2) {
            // Change the map to the one requested by the warpPoint
            bufferedWarpPoint = warpPoint;
            awaitingMoveFinish = true;
        } 
    }
}