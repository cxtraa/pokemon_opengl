#pragma once

#include <functional>

struct TilePosition {
    int x, y, layer;
    TilePosition(int x, int y, int layer) : x(x), y(y), layer(layer) {}
    TilePosition() {}

    bool operator==(const TilePosition& other) const {
        return x == other.x && y == other.y && layer == other.layer;
    }
};

namespace std {
    template <>
    struct hash<TilePosition> {
        std::size_t operator()(const TilePosition& p) const {
            return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1) ^ (std::hash<int>()(p.layer) << 2);
        }
    };
}