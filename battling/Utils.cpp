
#include "Utils.hpp"
#include <random>
#include <algorithm>

int Utils::randInt(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(a, b);
    return distrib(gen);
}

float Utils::stdRand() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0, 1);
    return distrib(gen);
}

std::vector<int> Utils::chooseK(int k, int n) {
    std::vector<int> answer;
    if (k > n) {
        for (int i = 0; i < n; i++) {
            answer.push_back(i);
        }
        return answer;
    }
    for (int i = 0; i < k; i++) {
        int genIdx = randInt(0, n-1);
        while (std::find(answer.begin(), answer.end(), genIdx) != answer.end()) {
            genIdx = randInt(0, n-1);
        }
        answer.push_back(genIdx);
    }
    return answer;
}