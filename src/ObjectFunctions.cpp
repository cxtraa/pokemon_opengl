
#include "ObjectFunctions.hpp"
#include "Utils.hpp"
#include "ItemName.hpp"
#include "PackSection.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::unordered_map<std::string, ObjectFunc>& ObjectFunctions::getRegistry() {
    static std::unordered_map<std::string, ObjectFunc> registry = []() {
        std::unordered_map<std::string, ObjectFunc> reg;
        
        // Manually register all the functions here
        // ...
        reg["pickUpItem"] = pickUpItem;

        return reg;
    }();
    return registry;
}

void ObjectFunctions::invoke(Object* object, std::string funcName) {
    std::unordered_map<std::string, ObjectFunc>& registry = getRegistry();
    if (registry.find(funcName) != registry.end()) {
        registry[funcName](object);
    }
    else {
        throw std::runtime_error("ObjectFunctions: the onInteract function " + funcName + " does not exist in the registry.");
    }
}

// ALL INTERACT FUNCTIONS GO HERE
void ObjectFunctions::pickUpItem(Object* object) {
    Game& game = Game::getInstance();
    Player& player = game.player;
    
    const json& data = object->data;
    std::string name = Utils::getProperty<std::string>(data, "name");
    int quantity = Utils::getProperty<int>(data, "quantity");

    player.pack.addItem(PackSection::ITEMS, name, quantity);
    int objectId = data["id"].get<int>();
    game.state.idOfCollectedItems.insert(objectId);

    // remove the item
    game.map.removeObject(object->x, object->y, object->layer);

    // player.pack.consoleLogPack();
    std::string textToDisplay = "Found " + std::to_string(quantity) + " " + (quantity > 1 ? name + "s" : name) + "!";
    game.dialogueManager.display(textToDisplay);
}