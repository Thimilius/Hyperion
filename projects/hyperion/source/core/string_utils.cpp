//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/string_utils.hpp"

//---------------------- Library Includes ----------------------
#ifdef HYP_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Array<uint32> StringUtils::GetCodepointsFromUtf8(const String &string) {
        // Implementation from https://github.com/sheredom/utf8.h/blob/master/utf8.h#L1145.
        const char *s = string.c_str();

        Array<uint32> codepoints;

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
            codepoints.Add(codepoint);
        }

        return codepoints;
    }

    //--------------------------------------------------------------
    String StringUtils::GetUtf8FromCodepoint(uint32 codepoint) {
        String result;

        if (0 == (0xffffff80 & codepoint)) {
            // 1-byte/7-bit ascii
            // (0b0xxxxxxx)
            result.resize(1);
            char *data = result.data();
            data[0] = static_cast<char>(codepoint);
        } else if (0 == (0xfffff800 & codepoint)) {
            // 2-byte/11-bit utf8 code point
            // (0b110xxxxx 0b10xxxxxx)
            result.resize(2);
            char *data = result.data();
            data[0] = static_cast<char>(0xc0 | static_cast<char>((codepoint >> 6) & 0x1f));
            data[1] = static_cast<char>(0xc0 | static_cast<char>((codepoint >> 6) & 0x1f));
        } else if (0 == (0xffff0000 & codepoint)) {
            // 3-byte/16-bit utf8 code point
            // (0b1110xxxx 0b10xxxxxx 0b10xxxxxx)
            result.resize(3);
            char *data = result.data();
            data[0] = static_cast<char>(0xe0 | static_cast<char>((codepoint >> 12) & 0x0f));
            data[1] = static_cast<char>(0x80 | static_cast<char>((codepoint >> 6) & 0x3f));
            data[2] = static_cast<char>(0x80 | static_cast<char>((codepoint) & 0x3f));
        } else if (0 == (0xffe00000 & codepoint)) {
            // 4-byte/21-bit utf8 code point
            // (0b11110xxx 0b10xxxxxx 0b10xxxxxx 0b10xxxxxx)
            result.resize(4);
            char *data = result.data();
            data[0] = static_cast<char>(0xf0 | static_cast<char>((codepoint >> 18) & 0x07));
            data[1] = static_cast<char>(0x80 | static_cast<char>((codepoint >> 12) & 0x3f));
            data[2] = static_cast<char>(0x80 | static_cast<char>((codepoint >> 6) & 0x3f));
            data[3] = static_cast<char>(0x80 | static_cast<char>((codepoint) & 0x3f));
        }

        return result;
    }

    //--------------------------------------------------------------
    String StringUtils::GetUtf8FromCodepoints(const Array<uint32> &codepoints) {
        String result;
        for (uint32 codepoint : codepoints) {
            result += GetUtf8FromCodepoint(codepoint);
        }
        return result;
    }

    //--------------------------------------------------------------
    uint32 StringUtils::GetLastUtf8CodepointSize(const String &string) {
        uint32 codepoint_size = 0;
        auto it = string.rbegin();
        while (it != string.end()) {
            codepoint_size++;
            char c = *it;
            if ((c & 0b11000000) != 0b10000000) {
                break;
            }
            it++;
        }
        return codepoint_size;
    }

    //--------------------------------------------------------------
    WideString StringUtils::Utf8ToUtf16(const String &string) {
#ifdef HYP_PLATFORM_WINDOWS
        int32 string_length = static_cast<int32>(string.length());
        auto utf16_length = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), string_length, nullptr, 0);

        Array<WCHAR> buffer(utf16_length + 1);
        buffer[utf16_length] = 0;

        MultiByteToWideChar(CP_UTF8, 0, string.c_str(), string_length, buffer.GetData(), utf16_length);

        WideString result = buffer.GetData();
        return result;
#else
        #error Missing platform implementation
#endif
    }

    //--------------------------------------------------------------
    String StringUtils::Utf16ToUtf8(const WideString &string) {
#ifdef HYP_PLATFORM_WINDOWS
        int32 string_length = static_cast<int32>(string.length());
        auto utf8_length = WideCharToMultiByte(CP_UTF8, 0, string.c_str(), string_length, nullptr, 0, nullptr, nullptr);

        Array<char> buffer(utf8_length + 1);
        buffer[utf8_length] = 0;

        WideCharToMultiByte(CP_UTF8, 0, string.c_str(), string_length, buffer.GetData(), utf8_length, nullptr, nullptr);

        String result = buffer.GetData();
        return result;
#else
        #error Missing platform implementation
#endif
    }

}
