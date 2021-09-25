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
    Array<byte> FileSystem::ReadAllBytes(const String &path) {
        HYP_PROFILE_SCOPE("FileSystem.ReadAllBytes");

        Array<byte> result;

        std::ifstream file(path, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            HYP_LOG_ERROR("Engine", "Failed to open file: '{}'!", GetAbsoluteFilePath(path));
            return result;
        }

        uint64 size = file.tellg();
        result.Resize(size);
        file.seekg(0);
        file.read(reinterpret_cast<char *>(result.GetData()), size);

        return result;
    }

    //--------------------------------------------------------------
    String FileSystem::ReadAllText(const String &path) {
        HYP_PROFILE_SCOPE("FileSystem.ReadAllText");

        std::ifstream file(path, std::ios::ate);
        if (!file.is_open()) {
            HYP_LOG_ERROR("Engine", "Failed to open file: '{}'!", GetAbsoluteFilePath(path));
            return String();
        }

        uint64 size = file.tellg();
        String buffer(size, ' ');
        file.seekg(0);
        file.read(buffer.data(), size);

        return buffer;
    }

    //--------------------------------------------------------------
    Array<String> FileSystem::ReadAllLines(const String &path) {
        HYP_PROFILE_SCOPE("FileSystem.ReadAllLines");

        Array<String> result;

        std::ifstream file(path);
        if (!file.is_open()) {
            HYP_LOG_ERROR("Engine", "Failed to open file: '{}'!", GetAbsoluteFilePath(path));
            return result;
        }

        String line;
        while (std::getline(file, line)) {
            result.Add(std::move(line));
        }

        return result;
    }

    //--------------------------------------------------------------
    void FileSystem::WriteAllText(const String &path, const String &text) {
        HYP_PROFILE_SCOPE("FileSystem.WriteAllText");

        std::ofstream file(path);
        file.write(text.data(), text.length());
    }

    //--------------------------------------------------------------
    void FileSystem::Delete(const String &path) {
        HYP_PROFILE_SCOPE("FileSystem.Delete");

        std::filesystem::remove(path);
    }

    //--------------------------------------------------------------
    bool8 FileSystem::Exists(const String &path) {
        HYP_PROFILE_SCOPE("FileSystem.Exists");

        return std::filesystem::exists(path);
    }

    //--------------------------------------------------------------
    String FileSystem::GetAbsoluteFilePath(const String& path) {
        HYP_PROFILE_SCOPE("FileSystem.GetAbsoluteFilePath");

        return std::filesystem::absolute(path).string();
    }

}