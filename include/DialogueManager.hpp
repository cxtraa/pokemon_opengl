#pragma once

#include "SpriteAnimation.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class DialogueManager {
public:
    float timeBetweenChars;
    float lastCharTime;
    DialogueManager();
    void display(std::string text);
    static std::vector<std::pair<int, int>> formatText(std::string text);
    static std::vector<std::string> splitBySpace(std::string text);
    void push();

    SpriteAnimation dialogueBox;
    SpriteAnimation awaitContinueArrow;
    bool displayingText;
    bool awaitingUserContinue;
    bool gotInput;
    std::string textToDisplay;
    std::vector<SpriteAnimation> buffer;
    std::vector<std::pair<int, int>> positions;
    int idx;

    std::unordered_map<std::string, int> charToId;
};