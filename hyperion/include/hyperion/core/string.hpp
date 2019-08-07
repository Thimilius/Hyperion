#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class CString {
    private:
        char *m_buffer;
        u32 m_length;
    public:
        CString();
        CString(bool value);
        CString(const char *format, ...);
        CString(const CString &string);
        ~CString();

        inline u32 GetLength() const { return m_length; };
        inline const char *ToCString() const { return m_buffer == nullptr ? "" : m_buffer; }

        static CString FromArgs(const char *format, va_list args);

        friend CString operator+(CString string, bool value);

        CString operator+(const CString &string) const;
        CString operator=(const CString &string);
    };

}