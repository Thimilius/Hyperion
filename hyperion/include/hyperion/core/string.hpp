#pragma once

#include "hyperion/common.hpp"
#include <vendor/fmt/format.h>

namespace Hyperion {

    class CString {
    private:
        char *m_buffer = nullptr;
        u32 m_length = 0;
    public:
        CString();
        CString(const char *string);
        CString(const CString &string);
        ~CString();

        inline u32 GetLength() const { return m_length; };
        inline const char *ToCString() const { return m_buffer == nullptr ? "" : m_buffer; }

        template<typename ...Args>
        static CString Format(const char *format, Args... args) {
            CString result;
            u32 length = (u32)fmt::formatted_size(format, args...);

            result.m_length = length;
            result.m_buffer = new char[length + 1];
            fmt::memory_buffer memory_buffer;
            fmt::format_to(memory_buffer, format, args...);

            memcpy(result.m_buffer, memory_buffer.data(), length);
            result.m_buffer[length] = 0;

            return result;
        }

        CString operator=(const CString &string);
    };

}

namespace fmt {

    template <>
    struct formatter<Hyperion::CString> {
        template <typename ParseContext>
        constexpr auto parse(ParseContext &context) { return context.begin(); }

        template <typename FormatContext>
        auto format(const Hyperion::CString &string, FormatContext &context) {
            return format_to(context.begin(), "{}", string.ToCString());
        }
    };

}