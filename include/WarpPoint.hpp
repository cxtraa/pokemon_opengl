#pragma once

#include "Direction.hpp"
#include <string>

struct WarpPoint {
    int x1, y1, x2, y2, targetX, targetY;
    std::string mapToWarpTo;
    Direction targetDir;
    WarpPoint() {};
    WarpPoint(int x1, int y1, int x2, int y2, int targetX, int targetY, Direction targetDir, std::string mapToWarpTo) : x1(x1), y1(y1), x2(x2), y2(y2), mapToWarpTo(mapToWarpTo), targetX(targetX), targetY(targetY), targetDir(targetDir) {}
};