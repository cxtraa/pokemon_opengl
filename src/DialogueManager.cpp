
#include "DialogueManager.hpp"
#include "constants.hpp"
#include "Game.hpp"
#include "Screen.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <iostream>

using json = nlohmann::json;

DialogueManager::DialogueManager() : 
    timeBetweenChars(CDialogue::charTime),
    lastCharTime(0.0f),
    idx(0),
    displayingText(false),
    awaitingUserContinue(false),
    gotInput(false)
{
    std::ifstream file(CJson::textPath);
    json j;
    file >> j;
    charToId = j.get<std::unordered_map<std::string, int>>();

    dialogueBox = SpriteAnimation(DIALOGUE, CDialogue::boxX, CDialogue::boxY, CLayers::boxLayer, 0.0f, false, std::vector<int>{0});
    awaitContinueArrow = SpriteAnimation(TEXT, CDialogue::contArrowX, CDialogue::contArrowY, 7.0f, 0.3f, false, std::vector<int>{charToId["AWAIT"], charToId[" "]});
}

void DialogueManager::display(std::string text) {
    if (displayingText) return; // wait until current text has been displayed
    displayingText = true;
    idx = 0;
    textToDisplay = text;
    positions = formatText(textToDisplay);
    positions.clear();
    buffer.clear();
    buffer.reserve(textToDisplay.length()); // Reserve capacity to prevent reallocation
    lastCharTime = glfwGetTime();
}

std::vector<std::pair<int, int>> DialogueManager::formatText(std::string text) {
    std::vector<std::pair<int, int>> answer;
    int offsetX = CDialogue::boxX + CDialogue::paddingX;
    int offsetY = CDialogue::boxY + CDialogue::paddingY;
    std::vector<std::string> words = DialogueManager::splitBySpace(text);
    int lines = 0;
    int chars = 0;
    for (int i = 0; i < words.size(); i++) {
        if (CDialogue::tileSize*(words[i].length() + 1 + chars) > CDialogue::totalWidth) {
            // Begin a new line
            chars = 0;
            lines++;
            if (lines + 1 > CDialogue::numLines) {
                // Begin a new page
                lines = 0;
            }
        }

        // Iterate through the word and place the positions
        int y = lines*CDialogue::tileSize + (lines+1)*CDialogue::tileSize + offsetY;
        for (int j = 0; j < words[i].length(); j++) {
            answer.push_back({
                (chars+j)*CDialogue::tileSize + offsetX,
                y
            });
        }
        // Place down the space position as well if its not the last word
        if (i != words.size() - 1) {
            answer.push_back({
                (chars+words[i].length())*CDialogue::tileSize + offsetX,
                y
            });
        }

        chars += words[i].length() + 1;
    }
    return answer;
}

std::vector<std::string> DialogueManager::splitBySpace(std::string text) {
    std::string currWord;
    std::vector<std::string> words;
    for (int i = 0; i < text.size(); i++) {
        if (text[i] == ' ') {
            words.push_back(currWord);
            currWord = "";
        }
        else {
            currWord += text[i];
        }
    }
    if (currWord.length() > 0) {
        words.push_back(currWord);
    }
    return words;
}

void DialogueManager::push() {
    if (awaitingUserContinue) {
        // Display the continue arrow at the bottom right
        Drawer& drawer = Game::getInstance().drawer;
        ButtonHandler& buttonHandler = Game::getInstance().buttonHandler;
        drawer.addSprite(&awaitContinueArrow, awaitContinueArrow.x, awaitContinueArrow.y);
        GLFWwindow* window = Screen::getInstance().getWindow();
        if (buttonHandler.isPressed(GLFW_KEY_Z)) {
            awaitingUserContinue = false;
            gotInput = true;
            buffer.clear();
        }
    }
    if (displayingText) {
        if (glfwGetTime() - lastCharTime > timeBetweenChars) {
            lastCharTime = glfwGetTime();
            
            if (idx >= textToDisplay.length()) {
                awaitingUserContinue = true;
                displayingText = false;
            }
            std::string newChar(1, textToDisplay[idx]);
            
            int charId = charToId[newChar];
            int x = positions[idx].first;
            int y = positions[idx].second;
            if (idx > 0) {
                int prevX = positions[idx-1].first;
                int prevY = positions[idx-1].second;
                if (y < prevY && !gotInput) {
                    awaitingUserContinue = true;
                    gotInput = false;
                }
            }
            if (!awaitingUserContinue) {
                SpriteAnimation newLetter(TEXT, x, y, 6.0f, 0.0f, false, std::vector<int>{charId});
                buffer.push_back(newLetter);
                idx++;
                gotInput = false;
            }
        }
    }
    
    // Continue displaying text even when waiting for user input
    if (awaitingUserContinue || displayingText) {
        Game& game = Game::getInstance();
        Drawer& drawer = game.drawer;
        for (int i = 0; i < buffer.size(); i++) {
            drawer.addSprite(&buffer[i], positions[i].first, positions[i].second);
        }
        drawer.addSprite(&dialogueBox, dialogueBox.x, dialogueBox.y);
    }
}