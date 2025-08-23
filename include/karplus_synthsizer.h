#pragma once
#include <vector>
#include <string>

class KarplusSynthesizer {
public:
    // synthesize принимает double-частоты и double-длительность
    std::vector<float> synthesize(
        const std::vector<double>& frequencies,
        int sampleRate,
        double duration,
        const std::string& instrument);
};
