#pragma once
#include <string>
#include <cstdint> // ��� INT32_MIN, INT32_MAX

// ���������� ������ ������ �����
int readInt(const std::string& prompt,
    int minVal = INT32_MIN,
    int maxVal = INT32_MAX);

// ���������� ������ ����� � ��������� ������
double readDouble(const std::string& prompt,
    double minVal = -1e9,
    double maxVal = 1e9);
