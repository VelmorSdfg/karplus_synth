#pragma once
#include <string>
#include <cstdint> // дл€ INT32_MIN, INT32_MAX

// Ѕезопасное чтение целого числа
int readInt(const std::string& prompt,
    int minVal = INT32_MIN,
    int maxVal = INT32_MAX);

// Ѕезопасное чтение числа с плавающей точкой
double readDouble(const std::string& prompt,
    double minVal = -1e9,
    double maxVal = 1e9);
