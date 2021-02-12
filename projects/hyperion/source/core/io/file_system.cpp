//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/io/file_system.hpp"

//--------------- C++ Standard Library Includes ----------------
#include <fstream>
#include <filesystem>

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
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

    //--------------------------------------------------------------
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

    //--------------------------------------------------------------
    void FileSystem::WriteAllText(const String &path, const String &text) {
        std::ofstream file(path);
        file.write(text.data(), text.length());
    }

    //--------------------------------------------------------------
    void FileSystem::Delete(const String &path) {
        std::filesystem::remove(path);
    }

}