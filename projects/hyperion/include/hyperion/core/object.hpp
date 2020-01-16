#pragma once

#include "hyperion/core/string_utils.hpp"

#define HYP_OBJECT(TYPE, BASE_TYPE)                                                                                            \
    private:                                                                                                                   \
        inline static ObjectType s_type = std::hash<String>()(String(#TYPE));                                                  \
    public:                                                                                                                    \
        inline static ObjectType GetStaticType() { return s_type; }                                                            \
        inline virtual ObjectType GetType() const override { return s_type; }                                                  \
        inline virtual bool IsBase(ObjectType type) const override { return type == s_type ? true : BASE_TYPE::IsBase(type); }
    
namespace Hyperion {

    using ObjectType = u64;

    class Object {
    private:
        String m_name;

        inline static ObjectType s_type = std::hash<String>()(String("Object"));
    protected:
        bool m_destroyed = false;
    public:
        inline static ObjectType GetStaticType() { return s_type; }
        inline virtual ObjectType GetType() const { return s_type; }
        inline virtual bool IsBase(ObjectType type) const { return type == s_type; }

        inline String GetName() const { return m_name; }
        inline void SetName(const String &name) { m_name = name; }

        inline virtual String ToString() const { return m_name; }

        static void Destroy(Object *object);
    protected:
        Object() = default;
        Object(const String &name) { m_name = name; }
        virtual ~Object() = default;

        // Objects can not be copied
        Object(const Object &other) = delete;
        Object &operator=(const Object &other) = delete;
    };

}