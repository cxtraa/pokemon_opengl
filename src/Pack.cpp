
#include <iostream>
#include "Pack.hpp"
#include "PackItemEntry.hpp"
#include "constants.hpp"
#include "Game.hpp"
#include "Database.hpp"
#include <GLFW/glfw3.h>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Pack::Pack() :
    visible(false),
    cancel(true),
    currOption(0),
    topItemIdx(0),
    selectArrowPositions(CPack::numOptions, std::pair<int,int>{})
{
    data = std::vector<std::unordered_map<std::string, PackItemEntry>>(PackSection::NUM_SECTIONS);
    std::unordered_map<std::string, int>& charToId = Database::getInstance().charToId;

    // Set the arrow positions to use for scrolling through
    for (int i = 0; i < selectArrowPositions.size(); i++) {
        selectArrowPositions[i] = {
            CPack::optionsOffsetX,
            CPack::optionsOffsetY + i*(CPack::tileSize + CPack::optionsSpacing)
        };
    }

    for (int i = 0; i < PackSection::NUM_SECTIONS; i++) {
        SpriteAnimation bagPicSprite(BAG_PICTURES, CPack::bagPicOffsetX, CPack::bagPicOffsetY, CLayers::textLayer, 0.0f, false, std::vector<int>{i});
        bagPictures.emplace_back(bagPicSprite);
        SpriteAnimation secTextSprite(SECTION_TEXTS, CPack::secTextOffsetX, CPack::secTextOffsetY, CLayers::textLayer, 0.0f, false, std::vector<int>{i});
        sectionTexts.emplace_back(secTextSprite);
    }

    background = SpriteAnimation(PACK_BACKGROUND, 0, 0, CLayers::boxLayer, 0.0f, false, std::vector<int>{0});
    selectArrow = SpriteAnimation(TEXT, CPack::arrowPosCancelX, CPack::arrowPosCancelY, CLayers::textLayer, 0.0f, false, std::vector<int>{charToId["SELECT"]});
}

void Pack::handleInput() {
    ButtonHandler& buttonHandler = Game::getInstance().buttonHandler;

    if (!visible) return;

    if (data[currentSection].empty()) cancel = true;

    int secIdx = static_cast<int>(currentSection);
    int nSec = static_cast<int>(PackSection::NUM_SECTIONS);

    if (buttonHandler.isPressed(GLFW_KEY_RIGHT)) {
        secIdx = (secIdx + 1) % nSec;
    }
    else if (buttonHandler.isPressed(GLFW_KEY_LEFT)) {
        secIdx = ((secIdx - 1 + nSec) % nSec);
    }
    currentSection = static_cast<PackSection>(secIdx);

    int itemCount = static_cast<int>(data[currentSection].size());
    int targetOption = currOption;

    if (buttonHandler.isPressed(GLFW_KEY_UP)) {
        if (cancel && !data[currentSection].empty()) {
            currOption = std::min(CPack::numOptions - 1, itemCount - 1);
            cancel = false;
            return;
        } else {
            targetOption--;
        }
    }
    else if (buttonHandler.isPressed(GLFW_KEY_DOWN)) {
        if (topItemIdx + currOption == itemCount - 1) {
            cancel = true;
            return;
        } else {
            targetOption++;
        }
    }
    else if (buttonHandler.isPressed(GLFW_KEY_Z) && cancel) {
        visible = false;
        return;
    }

    if (!data[currentSection].empty()) {
        if (!(0 <= targetOption && targetOption < CPack::numOptions)) {
            if (targetOption == -1) {
                topItemIdx = std::max(0, topItemIdx - 1);
            }
            else if (targetOption == CPack::numOptions) {
                topItemIdx = std::min(itemCount - 1, topItemIdx + 1);
            }
        } else {
            currOption = targetOption;
        }
    }
}

void Pack::addItem(PackSection section, std::string name, int quantity) {
    int secIdx = static_cast<int>(section);
    if (data[secIdx].find(name) == data[secIdx].end()) {
        data[secIdx][name] = PackItemEntry(name, quantity);
    }
    else {
        data[secIdx][name].quantity += quantity;
    }
}

void Pack::push() {
    handleInput();
    if (!visible) return;
    std::unordered_map<std::string, PackItemEntry>& items = data[currentSection];
    // Want to only render from topItemIdx down to min(actual items, 4). 
    // Take the entries and sort by alphabetical order on name
    std::vector<PackItemEntry*> itemEntries;
    for (auto& [name, itemEntry] : items) {
        itemEntries.emplace_back(&itemEntry);
    }
    std::sort(
        itemEntries.begin(),
        itemEntries.end(),
        [](PackItemEntry* a, PackItemEntry* b) {
            return a->name < b->name;
        }
    );
    Drawer& drawer = Game::getInstance().drawer;
    drawer.addSprite(&background, background.x, background.y);
    int bottomItemIdx = std::min(topItemIdx + CPack::numOptions, (int)items.size());
    for (int i = topItemIdx; i < bottomItemIdx; i++) {
        int row = i - topItemIdx;
        for (int j = 0; j < itemEntries[i]->textChars.size(); j++) {
            SpriteAnimation* c = itemEntries[i]->textChars[j].get();
            c->x = CPack::optionsOffsetX + (j+1)*(CPack::tileSize);
            c->y = CPack::optionsOffsetY + row*(CPack::tileSize + CPack::optionsSpacing);
            drawer.addSprite(c, c->x, c->y);
        }
        for (int j = 0; j < itemEntries[i]->quantityChars.size(); j++) {
            SpriteAnimation* c = itemEntries[i]->quantityChars[j].get();
            c->x = CPack::optionsOffsetX + CPack::quantityOffsetX + j*CPack::tileSize;
            c->y = CPack::optionsOffsetY + CPack::quantityOffsetY + row*(CPack::tileSize + CPack::optionsSpacing);
            drawer.addSprite(c, c->x, c->y); 
        }
    }

    // Render the right bag pic and section text
    drawer.addSprite(&bagPictures[currentSection], bagPictures[currentSection].x, bagPictures[currentSection].y);
    drawer.addSprite(&sectionTexts[currentSection], sectionTexts[currentSection].x, sectionTexts[currentSection].y);

    if (cancel) {
        drawer.addSprite(&selectArrow, CPack::arrowPosCancelX, CPack::arrowPosCancelY);
    }
    else {
        auto [selectX, selectY] = selectArrowPositions[currOption];
        drawer.addSprite(&selectArrow, selectX, selectY);
    }

    // We only want to render dialogue / select arrow IF we have items
    if (itemEntries.empty()) return;

    // Render dialogue for the selected entry
    PackItemEntry* currItem = itemEntries[topItemIdx + currOption];
    for (int i = 0; i < currItem->descChars.size(); i++) {
        SpriteAnimation* c = currItem->descChars[i].get();
        drawer.addSprite(c, c->x, c->y);
    }
}