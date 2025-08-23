#include "safe_input.h"
#include <iostream>
#include <string>
#include <stdexcept>

// Безопасный ввод целого числа
int readInt(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        try {
            size_t pos;
            int value = std::stoi(input, &pos);

            if (pos != input.size()) throw std::invalid_argument("Лишние символы");
            if (value < minVal || value > maxVal) throw std::out_of_range("Вне диапазона");

            return value;
        }
        catch (const std::exception&) {
            std::cout << "❌ Ошибка: введите целое число в диапазоне ["
                << minVal << " ,, " << maxVal << "]\n";
        }
    }
}

// Безопасный ввод числа с плавающей точкой
double readDouble(const std::string& prompt, double minVal, double maxVal) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        try {
            size_t pos;
            double value = std::stod(input, &pos);

            if (pos != input.size()) throw std::invalid_argument("Лишние символы");
            if (value < minVal || value > maxVal) throw std::out_of_range("Вне диапазона");

            return value;
        }
        catch (const std::exception&) {
            std::cout << "❌ Ошибка: введите число в диапазоне ["
                << minVal << " .. " << maxVal << "]\n";
        }
    }
}
