#pragma once
#include <vector>

class KarplusString {
public:
    KarplusString(double frequency, int sampleRate);
    float generate(); // сгенерировать следующий сэмпл
    void pluck();     // "дернуть струну"

private:
    std::vector<float> buffer;
    int index;
    int N;
};
