#pragma once

namespace Hyperion {

    class CString {
    private:
        char *m_buffer;
        int m_length;
    public:
        CString();
        CString(const char *format, ...);
        CString(const CString &string);
        ~CString();

        int GetLength() const { return m_length; };
        const char *ToCString() const { return m_buffer; }

        static CString FromArgs(const char *format, va_list args);

        CString operator+(const CString &string) const;
        CString operator=(const CString &string);
    };

}