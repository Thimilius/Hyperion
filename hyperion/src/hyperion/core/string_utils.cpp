#include "hyppch.hpp"

#include "hyperion/core/string_utils.hpp"

namespace Hyperion {

    TWString CStringUtils::Utf8ToUtf16(const TString &string) {
#ifdef HYP_PLATFORM_WINDOWS
        auto utf16_length = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.length(), nullptr, 0);

        WCHAR *buffer = new WCHAR[utf16_length + 1];
        buffer[utf16_length] = 0;

        MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.length(), buffer, utf16_length);

        std::wstring result = buffer;
        delete[] buffer;

        return result;
#else
    #error 
#endif
    }

    TString CStringUtils::Utf16ToUtf8(const TWString &string) {
#ifdef HYP_PLATFORM_WINDOWS
        auto utf8_length = WideCharToMultiByte(CP_UTF8, 0, string.c_str(), (int)string.length(), nullptr, 0, nullptr, nullptr);

        char *buffer = new char[utf8_length + 1];
        buffer[utf8_length] = 0;

        WideCharToMultiByte(CP_UTF8, 0, string.c_str(), (int)string.length(), buffer, utf8_length, nullptr, nullptr);

        TString result = buffer;
        delete[] buffer;

        return result;
#else
    #error 
#endif
    }

}