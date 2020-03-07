#include "hyppch.hpp"

#include "hyperion/core/string_utils.hpp"

#ifdef HYP_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

namespace Hyperion {

    Vector<u32> StringUtils::GetCodepointsUtf8(const String &string) {
        // Implementation from https://github.com/sheredom/utf8.h/blob/master/utf8.h
        const char *s = (const char *)string.c_str();

        Vector<u32> codepoints;

        // This assumes a valid utf8 string
        while (*s != '\0') {
            u32 codepoint;
            if (0xf0 == (0xf8 & s[0])) {
                // 4 byte utf8 codepoint
                codepoint = ((0x07 & s[0]) << 18) | ((0x3f & s[1]) << 12) | ((0x3f & s[2]) << 6) | (0x3f & s[3]);
                s += 4;
            } else if (0xe0 == (0xf0 & s[0])) {
                // 3 byte utf8 codepoint
                codepoint = ((0x0f & s[0]) << 12) | ((0x3f & s[1]) << 6) | (0x3f & s[2]);
                s += 3;
            } else if (0xc0 == (0xe0 & s[0])) {
                // 2 byte utf8 codepoint
                codepoint = ((0x1f & s[0]) << 6) | (0x3f & s[1]);
                s += 2;
            } else {
                // 1 byte utf8 codepoint otherwise
                codepoint = s[0];
                s += 1;
            }
            codepoints.push_back(codepoint);
        }

        return codepoints;
    }

    WideString StringUtils::Utf8ToUtf16(const String &string) {
#ifdef HYP_PLATFORM_WINDOWS
        s32 string_length = static_cast<s32>(string.length());
        auto utf16_length = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), string_length, nullptr, 0);

        WCHAR *buffer = new WCHAR[utf16_length + 1];
        buffer[utf16_length] = 0;

        MultiByteToWideChar(CP_UTF8, 0, string.c_str(), string_length, buffer, utf16_length);

        std::wstring result = buffer;
        delete[] buffer;

        return result;
#else
        #error Missing platform implementation
#endif
    }

    String StringUtils::Utf16ToUtf8(const WideString &string) {
#ifdef HYP_PLATFORM_WINDOWS
        s32 string_length = static_cast<s32>(string.length());
        auto utf8_length = WideCharToMultiByte(CP_UTF8, 0, string.c_str(), string_length, nullptr, 0, nullptr, nullptr);

        char *buffer = new char[utf8_length + 1];
        buffer[utf8_length] = 0;

        WideCharToMultiByte(CP_UTF8, 0, string.c_str(), string_length, buffer, utf8_length, nullptr, nullptr);

        String result = buffer;
        delete[] buffer;

        return result;
#else
        #error Missing platform implementation
#endif
    }

}
