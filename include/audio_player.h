#pragma once
#include <vector>

class AudioPlayer {
public:
    void play(const std::vector<float>& buffer, int sampleRate);
};
