#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/object/guid.hpp"

namespace Hyperion {
    class Engine;
    class Entity;
    class ObjectManager;
    class World;
    class WorldManager;
}

namespace Hyperion {

    using ObjectId = uint64;
    using ObjectGuid = Guid;

    class Object {
        HYP_REFLECT();
    public:
        // We would like to have the destructor private, but RTTR does not let us.
        // All objects should be destroyed through the static Object::Destroy function.
        virtual ~Object();

        inline Type GetType() const { return get_type(); }
        
        inline ObjectId GetId() const { return m_id; }
        inline ObjectGuid GetGuid() const { return m_guid; }

        inline String GetName() const { return m_name; }
        inline void SetName(const String &name) { m_name = name; }

        inline virtual String ToString() const { return m_name; }

        static Object *Create();
        static Object *Create(const String &name);
        static Object *Clone(Object *object);
        static void Destroy(Object *object);
    protected:
        Object();
        Object(const String &name);

        inline virtual Object *CreateClone() const { return new Object(); }
        virtual void HandleClone(Object *clone) const;

        virtual void OnDestroy() { }
    private:
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

    class ObjectManager final {
    public:
        static Object *Get(ObjectId object_id);
    private:
        ObjectManager() = delete;
        ~ObjectManager() = delete;

        static void LateUpdate();
        static void Shutdown();

        static void Destroy(Object *object);
        static void DestroyImmediate(Object *object);
        static void DestroyPendingObjects();

        static ObjectId RegisterObject(Object *object);
        static void UnregisterObject(Object *object);
    private:
        inline static Map<ObjectId, Object *> s_objects;
        inline static Set<Object *> s_objects_to_destroy;
        inline static ObjectId s_object_id_counter;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Object;
        friend class Hyperion::WorldManager;
    };

}