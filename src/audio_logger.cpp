#include "audio_logger.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

void AudioLogger::info(const std::string& msg) {
    std::cout << "[INFO] " << msg << std::endl;

#ifdef _WIN32
    OutputDebugStringA((msg + "\n").c_str());
#endif
}

void AudioLogger::error(const std::string& msg) {
    std::cerr << "[ERROR] " << msg << std::endl;

#ifdef _WIN32
    OutputDebugStringA((msg + "\n").c_str());
#endif
}
