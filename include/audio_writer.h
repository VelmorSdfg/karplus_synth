#pragma once
#include <string>
#include <vector>
#include <cstdint>

class AudioWriter {
public:
    static void writeToFile(const std::string& filename,
        const std::vector<int16_t>& samples,
        int sampleRate);
};
