#include <vector>
#include <iostream>

std::vector<std::string> splitBySpace(std::string text) {
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

std::vector<std::pair<int,int>> formatText(std::string text) {
    // given text `text`, we want to know where to place each character x and y anchor. 
    // return vector of positions. we will know to restart when the line position decreases rather than increases.
    std::vector<std::pair<int,int>> positions;
    int totalWidth = 120;
    int spacingBetweenLines = 8;
    int numLinesAllowed = 2;

    int offsetX = 0;
    int offsetY = 0;

    int letterWidth = 8;
    int letterHeight = 8;
    
    std::vector<std::string> words = splitBySpace(text);
    int lines = 0;
    int chars = 0;
    for (int i = 0; i < words.size(); i++) {
        if (letterWidth*(words[i].length() + 1 + chars) > totalWidth) {
            // Begin a new line
            chars = 0;
            lines++;
            if (lines >= numLinesAllowed) {
                lines = 0;
            }
        }

        // Iterate through the word and place the positions
        int y = lines*letterHeight + (lines+1)*spacingBetweenLines + offsetY;
        for (int j = 0; j < words[i].length(); j++) {
            positions.push_back({
                (chars+j)*letterWidth + offsetX,
                y
            });
        }
        // Place down the space position as well if its not the last word
        if (i != words.size() - 1) {
            positions.push_back({
                (chars+words[i].length())*letterWidth + offsetX,
                y
            });
        }

        chars += words[i].length() + 1;
    }

    return positions;
}

int main() {
    std::vector<std::pair<int,int>> positions = formatText("this is some example text that i wish to print out!");

    for (const std::pair<int,int>& pair : positions) {
        std::cout << pair.first << " " << pair.second << std::endl;
    }
}

