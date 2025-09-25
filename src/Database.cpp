
#include "Database.hpp"
#include "constants.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;

Database::Database() {
    json itemsJSON = getJSON(CJson::itemsPath);
    items = itemsJSON["items"].get<std::vector<json>>();
    
    json charToIdJSON = getJSON(CJson::textPath);
    charToId = charToIdJSON.get<std::unordered_map<std::string, int>>();
}

json Database::getJSON(std::string jsonPath) {
    std::ifstream file(jsonPath);
    json j;
    file >> j;
    return j;
}

Database& Database::getInstance() {
    static Database database;
    return database;
}