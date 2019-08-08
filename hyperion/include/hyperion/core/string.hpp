#pragma once

#include "hyperion/common.hpp"

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

        static CString FormatArgs(const char *format, va_list args);
        static CString Format(const char *format, ...);

        friend CString operator+(CString string, char *c_string);
        friend CString operator+(CString string, void *value);
        friend CString operator+(CString string, bool value);
        friend CString operator+(CString string, s8 value);
        friend CString operator+(CString string, u8 value);
        friend CString operator+(CString string, s16 value);
        friend CString operator+(CString string, u16 value);
        friend CString operator+(CString string, s32 value);
        friend CString operator+(CString string, u32 value);
        friend CString operator+(CString string, s64 value);
        friend CString operator+(CString string, u64 value);
        friend CString operator+(CString string, float value);
        friend CString operator+(CString string, double value);

        CString operator+(const CString &string) const;
        CString operator=(const CString &string);
    };

}