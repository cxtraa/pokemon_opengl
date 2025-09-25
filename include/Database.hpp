#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;

class Database {
public:
    std::vector<json> items;
    std::unordered_map<std::string, int> charToId;

    static Database& getInstance();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
private:
    Database();
    json getJSON(std::string jsonPath);
};