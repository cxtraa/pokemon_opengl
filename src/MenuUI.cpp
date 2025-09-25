
#include "MenuUI.hpp"
#include "Game.hpp"
#include "constants.hpp"
#include <GLFW/glfw3.h>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

MenuUI::MenuUI() :
    visible(false),
    currOption(0),
    optionTexts({"PACK", "OPTIONS", "EXIT", "CATO", "GORT"}),
    selectArrowPositions(optionTexts.size(), std::pair<int,int>{})
{
    std::ifstream file(CJson::textPath);
    json j;
    file >> j;
    charToId = j.get<std::unordered_map<std::string, int>>();
    
    for (int i = 0; i < selectArrowPositions.size(); i++) {
        selectArrowPositions[i] = {
            CMenu::menuBoxX + CMenu::tileSize,
            CMenu::menuBoxY + CMenu::paddingY + i*(CMenu::tileSize + CMenu::lineSpacing)
        };
    }

    background = SpriteAnimation(MENU, CMenu::menuBoxX, CMenu::menuBoxY, CLayers::boxLayer, 0.0f, false, std::vector<int>{0});
    selectArrow = SpriteAnimation(TEXT, selectArrowPositions[currOption].first, selectArrowPositions[currOption].second, CLayers::textLayer, 0.0f, false, std::vector<int>{charToId["SELECT"]});

    for (int i = 0; i < optionTexts.size(); i++) {
        for (int j = 0; j < optionTexts[i].length(); j++) {
            int anchorX = CMenu::menuBoxX + CMenu::paddingX;
            int anchorY = CMenu::menuBoxY + CMenu::paddingY;
            int x = anchorX + CMenu::tileSize*j;
            int y = anchorY + i*(CMenu::tileSize + CMenu::lineSpacing);
            int charId = charToId[std::string(1, optionTexts[i][j])];
            std::unique_ptr<SpriteAnimation> ptr = std::make_unique<SpriteAnimation>(TEXT, x, y, CLayers::textLayer, 0.0f, false, std::vector<int>{charId});
            chars.emplace_back(std::move(ptr));
        }
    }
}

void MenuUI::handleInput() {
    ButtonHandler& buttonHandler = Game::getInstance().buttonHandler;
    if (buttonHandler.isPressed(GLFW_KEY_ENTER)) {
        visible = true;
        return;
    }
    if (buttonHandler.isPressed(GLFW_KEY_X) && visible) {
        visible = false;
        return;
    }
    if (!visible) return;
    int n = optionTexts.size();
    if (buttonHandler.isPressed(GLFW_KEY_UP)) {
        currOption = ((currOption - 1) % n + n) % n;
    }
    else if (buttonHandler.isPressed(GLFW_KEY_DOWN)) {
        currOption = (currOption + 1) % n;
    }

    // Options
    if (optionTexts[currOption] == "PACK" && buttonHandler.isPressed(GLFW_KEY_Z)) {
        Game::getInstance().player.pack.visible = true;
        visible = false;
    }
}

void MenuUI::push() {
    handleInput();
    if (!visible) return;
    Drawer& drawer = Game::getInstance().drawer;
    drawer.addSprite(&background, background.x, background.y);
    auto [selectX, selectY] = selectArrowPositions[currOption];
    drawer.addSprite(&selectArrow, selectX, selectY);
    for (int i = 0; i < chars.size(); i++) {
        drawer.addSprite(chars[i].get(), chars[i]->x, chars[i]->y);
    }
}