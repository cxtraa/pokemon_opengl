#pragma once
#include <string>
#include "ItemName.hpp"
#include "SpriteAnimation.hpp"

class PackItemEntry {
public:
    std::string name;
    int quantity;
    std::string description;
    std::vector<std::unique_ptr<SpriteAnimation>> textChars;
    std::vector<std::unique_ptr<SpriteAnimation>> quantityChars;
    std::vector<std::unique_ptr<SpriteAnimation>> descChars;

    PackItemEntry(std::string name, int quantity);
    PackItemEntry();
};