#pragma once
#include <string>

class AudioLogger {
public:
    static void info(const std::string& msg);
    static void error(const std::string& msg);
};
