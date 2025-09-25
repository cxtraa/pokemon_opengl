
#include "Utils.hpp"
#include <random>

float Utils::randFloat() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float r = dist(gen);
    return r;
}