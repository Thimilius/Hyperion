#pragma once

#include <string>
#include "vendor/fmt/format.h"

namespace Hyperion {

    using TString = std::string;

    class CStringUtils {
    public:
        template<typename ...Args>
        inline static TString Format(const char *format, Args... args) {
            return fmt::format(format, args...);
        }
    private:
        CStringUtils() = delete;
        ~CStringUtils() = delete;
    };

}