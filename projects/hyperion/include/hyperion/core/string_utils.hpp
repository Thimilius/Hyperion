#pragma once

#include <string>
#include "fmt/format.h"

namespace Hyperion {

    using String = std::string;

    using WideString = std::wstring;

    class StringUtils {
    public:
        template<typename ...Args>
        inline static String Format(const char *format, Args... args) {
            return fmt::format(format, args...);
        }

        template<typename ...Args>
        inline static String Format(const String &format, Args... args) {
            return fmt::format(format, args...);
        }

        inline static bool EndsWith(String const &string, String const &ending) {
            if (string.length() >= ending.length()) {
                return string.compare(string.length() - ending.length(), ending.length(), ending) == 0;
            } else {
                return false;
            }
        }

        static WideString Utf8ToUtf16(const String &string);
        static String Utf16ToUtf8(const WideString &string);
    private:
        StringUtils() = delete;
        ~StringUtils() = delete;
    };

}