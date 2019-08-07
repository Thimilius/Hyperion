#include "hyppch.hpp"

#include "hyperion/core/string.hpp"

namespace Hyperion {

    CString::CString() : m_buffer(nullptr), m_length(0) { }

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

    CString CString::Format(const char *format, ...) {
        CString result;
        va_list args;
        va_start(args, format);
        result.m_length = vsnprintf(nullptr, 0, format, args);
        result.m_buffer = new char[result.m_length + 1];
        vsnprintf(result.m_buffer, result.m_length + 1, format, args);
        va_end(args);
        return result;
    }

    CString CString::FormatArgs(const char *format, va_list args) {
        CString result;
        result.m_length = vsnprintf(nullptr, 0, format, args);
        result.m_buffer = new char[result.m_length + 1];
        vsnprintf(result.m_buffer, result.m_length + 1, format, args);
        return result;
    }

    CString CString::operator+(const CString &string) const {
        CString result;
        result.m_length = m_length + string.m_length;
        result.m_buffer = new char[result.m_length + 1];
        strcpy(result.m_buffer, m_buffer);
        strcat(result.m_buffer, string.m_buffer);
        return result;
    }

    CString operator+(CString string, char *c_string) {
        CString result;
        result.m_length = string.m_length + (u32)strlen(c_string);
        result.m_buffer = new char[result.m_length + 1];
        strcpy(result.m_buffer, string.m_buffer);
        strcat(result.m_buffer, c_string);
        return result;
    }

    CString operator+(CString string, void *value) {
        CString result;
        result.m_length = snprintf(nullptr, 0, "%s%p", string.ToCString(), value);
        result.m_buffer = new char[result.m_length + 1];
        snprintf(result.m_buffer, result.m_length + 1, "%s%p", string.ToCString(), value);
        return result;
    }

    CString operator+(CString string, bool value) {
        CString result;
        result.m_length = string.m_length + (value ? 4 : 5);
        result.m_buffer = new char[result.m_length + 1];
        strcpy(result.m_buffer, string.m_buffer);
        strcat(result.m_buffer, value ? "true" : "false");
        return result;
    }

    CString operator+(CString string, s8 value) {
        CString result;
        result.m_length = snprintf(nullptr, 0, "%s%hhd", string.ToCString(), value);
        result.m_buffer = new char[result.m_length + 1];
        snprintf(result.m_buffer, result.m_length + 1, "%s%hhd", string.ToCString(), value);
        return result;
    }

    CString operator+(CString string, u8 value) {
        CString result;
        result.m_length = snprintf(nullptr, 0, "%s%c", string.ToCString(), value);
        result.m_buffer = new char[result.m_length + 1];
        snprintf(result.m_buffer, result.m_length + 1, "%s%c", string.ToCString(), value);
        return result;
    }
    
    CString operator+(CString string, s16 value) {
        CString result;
        result.m_length = snprintf(nullptr, 0, "%s%dh", string.ToCString(), value);
        result.m_buffer = new char[result.m_length + 1];
        snprintf(result.m_buffer, result.m_length + 1, "%s%dh", string.ToCString(), value);
        return result;
    }
    
    CString operator+(CString string, u16 value) {
        CString result;
        result.m_length = snprintf(nullptr, 0, "%s%hu", string.ToCString(), value);
        result.m_buffer = new char[result.m_length + 1];
        snprintf(result.m_buffer, result.m_length + 1, "%s%hu", string.ToCString(), value);
        return result;
    }
    
    CString operator+(CString string, s32 value) {
        CString result;
        result.m_length = snprintf(nullptr, 0, "%s%d", string.ToCString(), value);
        result.m_buffer = new char[result.m_length + 1];
        snprintf(result.m_buffer, result.m_length + 1, "%s%d", string.ToCString(), value);
        return result;
    }

    CString operator+(CString string, u32 value) {
        CString result;
        result.m_length = snprintf(nullptr, 0, "%s%u", string.ToCString(), value);
        result.m_buffer = new char[result.m_length + 1];
        snprintf(result.m_buffer, result.m_length + 1, "%s%u", string.ToCString(), value);
        return result;
    }

    CString operator+(CString string, s64 value) {
        CString result;
        result.m_length = snprintf(nullptr, 0, "%s%lld", string.ToCString(), value);
        result.m_buffer = new char[result.m_length + 1];
        snprintf(result.m_buffer, result.m_length + 1, "%s%lld", string.ToCString(), value);
        return result;
    }

    CString operator+(CString string, u64 value) {
        CString result;
        result.m_length = snprintf(nullptr, 0, "%s%llu", string.ToCString(), value);
        result.m_buffer = new char[result.m_length + 1];
        snprintf(result.m_buffer, result.m_length + 1, "%s%llu", string.ToCString(), value);
        return result;
    }

    CString operator+(CString string, float value) {
        CString result;
        result.m_length = snprintf(nullptr, 0, "%s%f", string.ToCString(), value);
        result.m_buffer = new char[result.m_length + 1];
        snprintf(result.m_buffer, result.m_length + 1, "%s%f", string.ToCString(), value);
        return result;
    }

    CString operator+(CString string, double value) {
        CString result;
        result.m_length = snprintf(nullptr, 0, "%s%Lf", string.ToCString(), value);
        result.m_buffer = new char[result.m_length + 1];
        snprintf(result.m_buffer, result.m_length + 1, "%s%Lf", string.ToCString(), value);
        return result;
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