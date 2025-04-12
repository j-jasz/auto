#include "utils.hpp"
#include <cstdlib>

std::string getHomeDir() {
    const char* homeDir = std::getenv("HOME");
    return homeDir ? std::string(homeDir) : std::string();
}
