#pragma once

#include "vendor/fmt/format.h"

namespace Hyperion {

    using CString = std::string;

    class CStringUtils {
    public:
        template<typename ...Args>
        inline static CString Format(const char *format, Args... args) {
            return fmt::format(format, args...);
        }
    private:
        CStringUtils() = delete;
        ~CStringUtils() = delete;
    };

}