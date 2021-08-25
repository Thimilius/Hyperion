#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <string>

//---------------------- Library Includes ----------------------
#include <fmt/format.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/collections/list.hpp"

//-------------------- Definition Namespace --------------------
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

        inline static constexpr uint32 Hash(const char *input) {
            return *input ? static_cast<uint32>(*input) + 33 * Hash(input + 1) : 5381;
        }

        static List<uint32> GetCodepointsUtf8(const String &string);

        static WideString Utf8ToUtf16(const String &string);
        static String Utf16ToUtf8(const WideString &string);
    private:
        StringUtils() = delete;
        ~StringUtils() = delete;
    };

}