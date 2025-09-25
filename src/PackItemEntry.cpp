
#include "PackItemEntry.hpp"
#include "constants.hpp"
#include "Database.hpp"
#include "DialogueManager.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;

PackItemEntry::PackItemEntry(std::string name, int quantity) :
    name(name),
    quantity(quantity)
{
    Database& db = Database::getInstance();
    for (int i = 0; i < db.items.size(); i++) {
        std::string itemName = db.items[i]["name"].get<std::string>();
        std::string itemDesc = db.items[i]["description"].get<std::string>();
        if (name == itemName) {
            description = itemDesc;
            break;
        }
    }

    // Init the characters (Their positions will be changed later when pushing)
    for (int i = 0; i < name.length(); i++) {
        int idx = db.charToId[std::string(1, name[i])];
        std::unique_ptr<SpriteAnimation> ptr = std::make_unique<SpriteAnimation>(TEXT, 0, 0, CLayers::textLayer, 0.0f, false, std::vector<int>{idx});
        textChars.emplace_back(std::move(ptr));
    }

    // Also do text for the quantity
    std::string quantityStr = "x" + std::to_string(quantity);
    for (int i = 0; i < quantityStr.size(); i++) {
        int idx = db.charToId[std::string(1, quantityStr[i])];
        std::unique_ptr<SpriteAnimation> ptr = std::make_unique<SpriteAnimation>(TEXT, 0, 0, CLayers::textLayer, 0.0f, false, std::vector<int>{idx});
        quantityChars.emplace_back(std::move(ptr));
    }

    // And the description as well
    std::vector<std::pair<int, int>> positions = DialogueManager::formatText(description);
    for (int i = 0; i < positions.size(); i++) {
        int x = positions[i].first;
        int y = positions[i].second;
        int idx = db.charToId[std::string(1, description[i])];
        std::unique_ptr<SpriteAnimation> ptr = std::make_unique<SpriteAnimation>(TEXT, x, y, CLayers::textLayer, 0.0f, false, std::vector<int>{idx});
        descChars.emplace_back(std::move(ptr));
    }
}

PackItemEntry::PackItemEntry() : name(), quantity(0) {}