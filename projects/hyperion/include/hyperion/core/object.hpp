#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/guid.hpp"

namespace Hyperion {
    class Entity;
    class ObjectManager;
    class World;
}

namespace Hyperion {

    using ObjectId = u64;
    using ObjectGuid = Guid;

    class Object {
        HYP_REFLECT();
    public:
        inline Type GetType() const { return get_type(); }
        
        inline ObjectId GetId() const { return m_id; }
        inline ObjectGuid GetGuid() const { return m_guid; }

        inline String GetName() const { return m_name; }
        inline void SetName(const String &name) { m_name = name; }

        inline virtual String ToString() const { return m_name; }

        static Object *Clone(Object *object);
        static void Destroy(Object *object);
    protected:
        Object();
        Object(const String &name);
        virtual ~Object();

        inline virtual Object *CreateClone() const { return new Object(); }
        virtual void HandleClone(Object *clone) const;

        virtual void OnDestroy() { }
    private:
        // Objects can not be copied
        Object(const Object &other) = delete;
        Object &operator=(const Object &other) = delete;

        static void DestroyImmediate(Object *object);
    private:
        ObjectId m_id = 0;
        ObjectGuid m_guid = Guid::Create();
        String m_name = "Object";
        bool m_destroyed = false;
    private:
        friend class Hyperion::Entity;
        friend class Hyperion::ObjectManager;
        friend class Hyperion::World;
    };

}