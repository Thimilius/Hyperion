#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class FileSystem final {
    public:
        static String ReadAllText(const String &path);
        static Vector<String> ReadAllLines(const String &path);
        static void WriteAllText(const String &path, const String &text);

        static void Delete(const String &path);
    private:
        FileSystem() = delete;
        ~FileSystem() = delete;
    };

}