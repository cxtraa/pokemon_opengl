#pragma once

#include "PackItemEntry.hpp"
#include "PackSection.hpp"
#include "ItemName.hpp"
#include "SpriteAnimation.hpp"
#include <vector>
#include <unordered_map>

class Pack {
public:
    std::vector<std::unordered_map<std::string, PackItemEntry>> data;
    std::vector<std::pair<int, int>> selectArrowPositions;
    SpriteAnimation background;
    std::vector<SpriteAnimation> bagPictures;
    std::vector<SpriteAnimation> sectionTexts;
    SpriteAnimation selectArrow;
    std::vector<std::unique_ptr<SpriteAnimation>> chars;

    bool visible;
    bool cancel;
    int currOption;
    int topItemIdx;
    PackSection currentSection;

    void handleInput();
    void push();
    void addItem(PackSection section, std::string name, int quantity);
    Pack();
};