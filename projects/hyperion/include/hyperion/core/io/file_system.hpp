#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class FileSystem final {
    public:
        static String ReadAllText(const String &path);
        static Vector<String> ReadAllLines(const String &path);
    private:
        FileSystem() = delete;
        ~FileSystem() = delete;
    };

}