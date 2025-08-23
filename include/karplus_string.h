#pragma once
#include <vector>

class KarplusString {
public:
    KarplusString(double frequency, int sampleRate);
    float generate(); // ������������� ��������� �����
    void pluck();     // "������� ������"

private:
    std::vector<float> buffer;
    int index;
    int N;
};
