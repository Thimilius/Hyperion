#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class FileUtilities {
    public:
        static String ReadTextFile(const String &path);
    private:
        FileUtilities() = delete;
        ~FileUtilities() = delete;
    };

}