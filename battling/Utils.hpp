#pragma once

#include <vector>
#include <iostream>

class Utils {
public:
    static int randInt(int a, int b);
    static float stdRand();
    static std::vector<int> chooseK(int k, int n);

    template <typename T>
    static void logVector(std::vector<T> vec, std::string delimiter) {
        std::cout << "[";
        for (int i = 0; i < vec.size() - 1; i++) {
            std::cout << vec[i] << delimiter;
        }
        std::cout << vec[vec.size()-1] << "]" << std::endl;;
    }
};