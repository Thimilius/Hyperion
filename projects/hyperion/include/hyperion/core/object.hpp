#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {
    class Entity;
    class ObjectManager;
    class World;
}

namespace Hyperion {

    class Object {
        HYP_REFLECT();
    public:
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
    private:
        friend class Hyperion::Entity;
        friend class Hyperion::ObjectManager;
        friend class Hyperion::World;
    };

}