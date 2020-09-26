#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class FileUtilities final {
    public:
        static String ReadAllText(const String &path);
        static Vector<String> ReadAllLines(const String &path);
    private:
        FileUtilities() = delete;
        ~FileUtilities() = delete;
    };

}