#include "hyppch.hpp"

#include "hyperion/core/io/file_system.hpp"

#include <fstream>

namespace Hyperion {

    String FileSystem::ReadAllText(const String &path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            HYP_LOG_ERROR("Engine", "Failed to open file: '{}'!", std::filesystem::absolute(path).u8string());
            return String();
        }

        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        std::string buffer(size, ' ');
        file.seekg(0);
        file.read(&buffer[0], size);

        return buffer;
    }

    Vector<String> FileSystem::ReadAllLines(const String &path) {
        Vector<String> result;

        std::ifstream file(path);
        if (!file.is_open()) {
            HYP_LOG_ERROR("Engine", "Failed to open file: '{}'!", std::filesystem::absolute(path).u8string());
            return result;
        }

        std::string line;
        while (std::getline(file, line)) {
            result.push_back(line);
        }

        return result;
    }

}