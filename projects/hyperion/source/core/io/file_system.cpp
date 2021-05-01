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
    Vector<uint8> FileSystem::ReadAllBytes(const String &path) {
        Vector<uint8> result;

        std::ifstream file(path, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            HYP_LOG_ERROR("Engine", "Failed to open file: '{}'!", std::filesystem::absolute(path).u8string());
            return result;
        }

        uint64 size = file.tellg();
        result.resize(size);
        file.seekg(0);
        file.read(reinterpret_cast<char *>(result.data()), size);

        return result;
    }

    //--------------------------------------------------------------
    String FileSystem::ReadAllText(const String &path) {
        std::ifstream file(path, std::ios::ate);
        if (!file.is_open()) {
            HYP_LOG_ERROR("Engine", "Failed to open file: '{}'!", std::filesystem::absolute(path).u8string());
            return String();
        }

        uint64 size = file.tellg();
        String buffer(size, ' ');
        file.seekg(0);
        file.read(buffer.data(), size);

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

        String line;
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