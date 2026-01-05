#pragma once
#include <random>

inline std::mt19937 rng{ std::random_device{}() };

inline int randomInt(int l, int r) {
    std::uniform_int_distribution<int> dist(l, r);
    return dist(rng);
}
inline float randomFloat(float l, float r) {
    std::uniform_real_distribution<float> dist(l, r);
    return dist(rng);
}