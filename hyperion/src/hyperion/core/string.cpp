#include "hyppch.hpp"

#include "hyperion/core/string.hpp"

namespace Hyperion {

    CString::CString() : m_buffer(nullptr), m_length(0) { }

    CString::CString(const char *format, ...) {
        va_list args;
        va_start(args, format);
        m_length = vsnprintf(nullptr, 0, format, args);
        m_buffer = new char[m_length + 1];
        vsnprintf(m_buffer, m_length + 1, format, args);
        va_end(args); 
    }

    CString::CString(const CString& string) {
        if (this == &string) {
            return;
        }
        
        m_length = string.GetLength();
        m_buffer = new char[m_length + 1];
        strcpy(m_buffer, string.m_buffer);
    }

    CString::~CString() {
        if (m_buffer) {
            delete[] m_buffer;
        }
    }

    CString CString::FromArgs(const char *format, va_list args) {
        CString result;
        result.m_length = vsnprintf(nullptr, 0, format, args);
        result.m_buffer = new char[result.m_length + 1];
        vsnprintf(result.m_buffer, result.m_length + 1, format, args);
        return result;
    }

    CString::operator const char*() {
        return m_buffer;
    }

    CString CString::operator+(const CString &string) const {
        CString result;
        result.m_buffer = new char[GetLength() + string.GetLength() + 1];
        strcpy(result.m_buffer, m_buffer);
        strcat(result.m_buffer, string.m_buffer);
        return result;
    }

    CString CString::operator=(const CString &string) {
        if (this != &string) {
            if (m_buffer) {
                delete[] m_buffer;
            }
            m_length = string.GetLength();
            m_buffer = new char[m_length + 1];
            strcpy(m_buffer, string.m_buffer);
        }
        return *this;
    }

}