#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class TypeInfo {
        const char *m_name;
        u64 m_size;
    public:
        TypeInfo(const char *name, u64 size)
            : m_name(name), m_size(size) { }
        virtual ~TypeInfo() { }

        String GetName() const { return m_name; }
        u64 GetSize() const { return m_size; }
    };

    class TypeDatabase {
    public:
        template<typename T>
        static void Register(const char* name) {
            
        }
    };

    void RegisterEngineTypes();
    void RegisterApplicationTypes();

}