#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class FileSystem final {
    public:
        static Array<byte> ReadAllBytes(const String &path);
        static String ReadAllText(const String &path);
        static Array<String> ReadAllLines(const String &path);

        static void WriteAllText(const String &path, const String &text);

        static void Delete(const String &path);
        static bool8 Exists(const String &path);

        static String GetAbsoluteFilePath(const String &path);
    private:
        FileSystem() = delete;
        ~FileSystem() = delete;
    };

}