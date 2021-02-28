#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/core/object/guid.hpp"
#include "hyperion/scripting/scripting_instance.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
    class ObjectManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using ObjectId = uint64;
    using ObjectGuid = Guid;

    class Object {
        HYP_REFLECT();
    public:
        // We would like to have the destructor private, but RTTR does not let us.
        // All objects should be destroyed through the static Object::Destroy function.
        virtual ~Object();
    public:
        inline Type GetType() const { return get_type(); }
        
        inline ObjectId GetId() const { return m_id; }
        inline ObjectGuid GetGuid() const { return m_guid; }

        inline bool IsDestroyed() const { return m_is_destroyed; }

        inline String GetName() const { return m_name; }
        inline void SetName(const String &name) { m_name = name; }

        inline Scripting::ScriptingInstance *GetScriptingInstance() const { return m_scripting_instance; }
        inline void SetScriptingInstance(Scripting::ScriptingInstance *scripting_instance) { m_scripting_instance = scripting_instance; }

        inline virtual String ToString() const { return m_name; }
    public:
        static Object *Create();
        static Object *Create(const String &name);

        static void Destroy(Object *object);
        static void DestroyImmediate(Object *object);
    protected:
        Object();
        Object(const String &name);
    protected:
        virtual void OnDestroy() { }
    private:
        Object(const Object &other) = delete;
        Object &operator=(const Object &other) = delete;
    private:
        ObjectId m_id = 0;
        ObjectGuid m_guid = Guid::Create();

        bool m_is_destroyed = false;

        String m_name = "Object";

        Scripting::ScriptingInstance *m_scripting_instance = nullptr;
    private:
        friend class Hyperion::ObjectManager;
    };

    class ObjectManager final {
    public:
        static Object *Get(ObjectId object_id);
    private:
        ObjectManager() = delete;
        ~ObjectManager() = delete;
    private:
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
    };

}