#include "hyppch.hpp"

#include "hyperion/core/string_utils.hpp"

#ifdef HYP_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

namespace Hyperion {

    WideString StringUtils::Utf8ToUtf16(const String &string) {
#ifdef HYP_PLATFORM_WINDOWS
        auto utf16_length = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.length(), nullptr, 0);

        WCHAR *buffer = new WCHAR[utf16_length + 1];
        buffer[utf16_length] = 0;

        MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.length(), buffer, utf16_length);

        std::wstring result = buffer;
        delete[] buffer;

        return result;
#else
        #error Missing platform implementation
#endif
    }

    String StringUtils::Utf16ToUtf8(const WideString &string) {
#ifdef HYP_PLATFORM_WINDOWS
        auto utf8_length = WideCharToMultiByte(CP_UTF8, 0, string.c_str(), (int)string.length(), nullptr, 0, nullptr, nullptr);

        char *buffer = new char[utf8_length + 1];
        buffer[utf8_length] = 0;

        WideCharToMultiByte(CP_UTF8, 0, string.c_str(), (int)string.length(), buffer, utf8_length, nullptr, nullptr);

        String result = buffer;
        delete[] buffer;

        return result;
#else
        #error Missing platform implementation
#endif
    }

}
