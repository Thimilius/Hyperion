#include "hyppch.hpp"

#include "hyperion/core/string.hpp"

namespace Hyperion {

    CString::CString() { }

    CString::CString(const char *string) {
        m_length = (u32)strlen(string);
        m_buffer = new char[m_length + 1];
        strcpy(m_buffer, string);
    }

    CString::CString(const CString& string) {
        if (this == &string) {
            return;
        }
        
        m_length = string.m_length;
        m_buffer = new char[m_length + 1];
        strcpy(m_buffer, string.m_buffer);
    }

    CString::~CString() {
        if (m_buffer) {
            delete[] m_buffer;
        }
    }

    CString CString::operator=(const CString &string) {
        if (this != &string) {
            if (m_buffer) {
                delete[] m_buffer;
            }
            m_length = string.m_length;
            m_buffer = new char[m_length + 1];
            strcpy(m_buffer, string.m_buffer);
        }
        return *this;
    }

}