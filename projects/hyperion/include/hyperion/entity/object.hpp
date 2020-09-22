#pragma once

#include "hyperion/common.hpp"

#define HYP_OBJECT(TYPE, BASE_TYPE)                                                                                            \
    private:                                                                                                                   \
        inline static ObjectType s_type = std::hash<String>()(String(#TYPE));                                                  \
    public:                                                                                                                    \
        inline virtual ObjectType GetType() const override { return s_type; }                                                  \
        inline virtual bool IsBase(ObjectType type) const override { return type == s_type ? true : BASE_TYPE::IsBase(type); } \
        inline static ObjectType GetStaticType() { return s_type; }                                                            \
    private:
    
namespace Hyperion {
    class Entity;
    class ObjectManager;
    class World;
}

namespace Hyperion {

    using ObjectType = u64;

    class Object {
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

        virtual void OnDestroy() { }
    private:
        // Objects can not be copied
        Object(const Object &other) = delete;
        Object &operator=(const Object &other) = delete;

        static void DestroyImmediate(Object *object);
    private:
        String m_name;
        bool m_destroyed = false;

        inline static ObjectType s_type = std::hash<String>()(String("Object"));
    private:
        friend class Hyperion::Entity;
        friend class Hyperion::ObjectManager;
        friend class Hyperion::World;
    };

}