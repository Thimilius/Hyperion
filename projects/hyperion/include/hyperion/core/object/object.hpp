#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/core/object/guid.hpp"
#include "hyperion/core/object/non_copyable.hpp"
#include "hyperion/core/object/type.hpp"
#include "hyperion/core/serialization/serializable.hpp"
#include "hyperion/scripting/scripting_object.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
    class ObjectManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using ObjectId = uint64;
    using ObjectGuid = Guid;

    class Object : public Scripting::ScriptingObject, public INonCopyable, public ISerializable {
        HYP_REFLECT();
    public:
        inline ObjectId GetId() const { return m_id; }
        inline ObjectGuid GetGuid() const { return m_guid; }

        inline bool IsDestroyed() const { return m_is_destroyed; }

        inline String GetName() const { return m_name; }
        inline void SetName(const String &name) { m_name = name; }

        inline virtual String ToString() const { return m_name; }

        virtual void Serialize(ISerializationStream &stream) override;
        virtual void Deserialize(IDeserializationStream &stream) override;
    public:
        static Object *Create();
        static Object *Create(const String &name);

        static Object *Clone(Object *object);
        template<typename T> static T *Clone(T *object) { return static_cast<T *>(Clone(static_cast<Object *>(object))); }

        static void Destroy(Object *object);
        static void DestroyImmediate(Object *object);
    protected:
        Object();
        Object(const String &name);
        virtual ~Object();
    protected:
        virtual void OnClone(Object *clone);
        virtual void OnDestroy() { }
    private:
        ObjectId m_id = 0;
        ObjectGuid m_guid = Guid::Create();

        bool m_is_destroyed = false;

        String m_name = "Object";

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
        inline static Vector<Object *> s_objects_to_destroy;
        inline static ObjectId s_object_id_counter;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Object;
    };

}