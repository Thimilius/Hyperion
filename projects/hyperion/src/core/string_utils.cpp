#include "hyppch.hpp"

#include "hyperion/core/string_utils.hpp"

#ifdef HYP_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

namespace Hyperion {

    Vector<uint32> StringUtils::GetCodepointsUtf8(const String &string) {
        // Implementation from https://github.com/sheredom/utf8.h/blob/master/utf8.h.
        const char *s = string.c_str();

        Vector<uint32> codepoints;

        // This assumes a valid utf8 string.
        while (*s != '\0') {
            uint32 codepoint;
            if (0xF0 == (0xF8 & s[0])) {
                // 4 byte utf8 codepoint
                codepoint = ((0x07 & s[0]) << 18) | ((0x3F & s[1]) << 12) | ((0x3F & s[2]) << 6) | (0x3F & s[3]);
                s += 4;
            } else if (0xE0 == (0xF0 & s[0])) {
                // 3 byte utf8 codepoint
                codepoint = ((0x0F & s[0]) << 12) | ((0x3F & s[1]) << 6) | (0x3F & s[2]);
                s += 3;
            } else if (0xC0 == (0xE0 & s[0])) {
                // 2 byte utf8 codepoint
                codepoint = ((0x1F & s[0]) << 6) | (0x3F & s[1]);
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
        int32 string_length = static_cast<int32>(string.length());
        auto utf16_length = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), string_length, nullptr, 0);

        Vector<WCHAR> buffer(utf16_length + 1);
        buffer[utf16_length] = 0;

        MultiByteToWideChar(CP_UTF8, 0, string.c_str(), string_length, buffer.data(), utf16_length);

        WideString result = buffer.data();
        return result;
#else
        #error Missing platform implementation
#endif
    }

    String StringUtils::Utf16ToUtf8(const WideString &string) {
#ifdef HYP_PLATFORM_WINDOWS
        int32 string_length = static_cast<int32>(string.length());
        auto utf8_length = WideCharToMultiByte(CP_UTF8, 0, string.c_str(), string_length, nullptr, 0, nullptr, nullptr);

        Vector<char> buffer(utf8_length + 1);
        buffer[utf8_length] = 0;

        WideCharToMultiByte(CP_UTF8, 0, string.c_str(), string_length, buffer.data(), utf8_length, nullptr, nullptr);

        String result = buffer.data();
        return result;
#else
        #error Missing platform implementation
#endif
    }

}
