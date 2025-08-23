#pragma once
#include <stdexcept>
#include <string>

class AudioError : public std::runtime_error {
public:
    explicit AudioError(const std::string& msg) : std::runtime_error(msg) {}
};
