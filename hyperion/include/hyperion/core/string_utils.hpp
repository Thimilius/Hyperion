#pragma once

#include <string>
#include "vendor/fmt/format.h"

namespace Hyperion {

    using TString = std::string;

    using TWString = std::wstring;

    class CStringUtils {
    public:
        template<typename ...Args>
        inline static TString Format(const char *format, Args... args) {
            return fmt::format(format, args...);
        }

        inline static bool EndsWith(TString const &string, TString const &ending) {
            if (string.length() >= ending.length()) {
                return string.compare(string.length() - ending.length(), ending.length(), ending) == 0;
            } else {
                return false;
            }
        }

        static TWString Utf8ToUtf16(const TString &string);
        static TString Utf16ToUtf8(const TWString &string);
    private:
        CStringUtils() = delete;
        ~CStringUtils() = delete;
    };

}