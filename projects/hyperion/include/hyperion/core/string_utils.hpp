#pragma once

#include <string>

#include <fmt/format.h>

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

        inline static String GetExtension(const String &string) {
            size_t position = string.find_last_of('.');
            if (position != string.npos) {
                return string.substr(position);
            }
            return string;
        }

        inline static constexpr u32 Hash(char const *input) {
            return *input ? static_cast<u32>(*input) + 33 * Hash(input + 1) : 5381;
        }

        static Vector<u32> GetCodepointsUtf8(const String &string);

        static WideString Utf8ToUtf16(const String &string);
        static String Utf16ToUtf8(const WideString &string);
    private:
        StringUtils() = delete;
        ~StringUtils() = delete;
    };

}