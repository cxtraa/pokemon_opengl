#pragma once

#include <string>

const std::string appName = "Pokemon";

namespace CScreen {
    const int screenSF = 6;
    const int numTilesX = 11;
    const int numTilesY = 9;
    const int tileSize = 16;
    const int pixelsWidth = tileSize * numTilesX;
    const int pixelsHeight = tileSize * numTilesY;
    const int screenWidth = pixelsWidth * screenSF;
    const int screenHeight = pixelsHeight * screenSF;
}

namespace CShader {
    const std::string vertexPath = "./shaders/vertex.shader";
    const std::string fragmentPath = "./shaders/fragment.shader";
}

namespace CJson {
    const std::string characterPath = "./data/characters.json";
    const std::string textPath = "./data/text.json";
    const std::string mapPath = "./tiled_project/map_1.json";
    const std::string tilesetPath = "./data/tilesets.json";
    const std::string mapsDir = "./tiled_project";
    const std::string itemsPath = "./data/items.json";
}

namespace CCharacter {
    const int characterSpeed = 1;
    const float characterLayer = 0.5;
    const float characterFrameDuration = 0.2f;
    const float characterTileTime = 0.4f;
};

namespace CPlayer {
    const float playerWalkTileTime = 0.4f;
    const float playerRunTileTime = 0.2f;
}

namespace CPokemonEncounter {
    const float pEncounter = 0.05;
}

namespace CDialogue {
    const int tileSize = 8;
    const int boxX = 0;
    const int boxY = CScreen::pixelsHeight - 6*tileSize;

    const int contArrowTileOffsetX = 20;
    const int contArrowTileOffsetY = 5;
    const int contArrowOffsetX = contArrowTileOffsetX*tileSize;
    const int contArrowOffsetY = contArrowTileOffsetY*tileSize;

    const int contArrowX = contArrowOffsetX + boxX;
    const int contArrowY = contArrowOffsetY + boxY;

    const int totalWidth = CScreen::pixelsWidth - 2*tileSize;
    const int lineSpacing = tileSize;
    const int paddingX = tileSize;
    const int paddingY = tileSize;
    const int numLines = 2;

    const float charTime = 0.05f;
}

namespace CMenu {
    const int width = 80;
    const int height = 128;
    const int tileSize = 8;
    const int menuBoxX = CScreen::pixelsWidth - width;
    const int menuBoxY = 0;
    const int paddingX = 2*tileSize;
    const int paddingY = 2*tileSize;
    const int lineSpacing = tileSize;
}

namespace CPack {
    const int numOptions = 4;
    const int tileSize = 8;

    const int optionsTilesOffsetX = 6;
    const int optionsTilesOffsetY = 2;
    const int optionsOffsetX = tileSize * optionsTilesOffsetX;
    const int optionsOffsetY = tileSize * optionsTilesOffsetY;

    const int optionsTilesSpacing = 1;
    const int optionsSpacing = tileSize * optionsTilesSpacing;

    const int quantityTilesOffsetX = 13;
    const int quantityTilesOffsetY = 1;
    const int quantityOffsetX = quantityTilesOffsetX * tileSize;
    const int quantityOffsetY = quantityTilesOffsetY * tileSize;

    const int bagPicOffsetX = 0;
    const int bagPicOffsetY = 3 * tileSize;
    const int secTextOffsetX = 0;
    const int secTextOffsetY = 7 * tileSize;

    const int arrowPosCancelX = 9 * tileSize;
    const int arrowPosCancelY = 10 * tileSize;
}

namespace CLayers {
    const float textLayer = 6;
    const float boxLayer = 5;
}

namespace CMap {
    const int maxLayers = 10;
}

namespace CInput {
    const float cooldownTime = 0.2f;
}