#pragma once

#include "SpriteAnimation.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

class MenuUI {
public:
    std::vector<std::string> optionTexts;
    std::vector<std::pair<int, int>> selectArrowPositions;
    std::unordered_map<std::string, int> charToId;
    SpriteAnimation background;
    SpriteAnimation selectArrow;

    std::vector<std::unique_ptr<SpriteAnimation>> chars;
    bool visible;
    int currOption;
    void handleInput();
    MenuUI();

    void push();

};