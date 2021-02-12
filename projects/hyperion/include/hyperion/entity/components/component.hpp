#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/object/object.hpp"
#include "hyperion/scripting/scripting_type.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Entity;
    class Transform;
    class World;
    class WorldManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class ComponentType {
    public:
        ComponentType(const Type &native_type);
        ComponentType(Scripting::ScriptingType *scripting_type);
    public:
        inline Type GetNativeType() const { return m_native_type; }
        inline Scripting::ScriptingType *GetScriptingType() const { return m_scripting_type; }

        bool IsDerivedFrom(const ComponentType &base) const;

        bool operator==(const ComponentType &other) const;
        bool operator!=(const ComponentType &other) const;
    private:
        bool m_is_native_type;

        Type m_native_type;
        Scripting::ScriptingType *m_scripting_type;
    private:
        friend struct std::hash<Hyperion::ComponentType>;
    };

    class Component : public Object {
        HYP_REFLECT(Object);
    public:
        inline Entity *GetEntity() const { return m_entity; }

        Transform *GetTransform() const;
        World *GetWorld() const;
    protected:
        Component() = default;
        Component(const String &name) : Object(name) { }
    protected:
        virtual void OnCreate() { }
        virtual void OnDestroy() override { }
    private:
        Entity *m_entity;
    private:
        friend class Hyperion::Entity;
        friend class Hyperion::WorldManager;
    };

}

namespace std {

    template <>
    struct hash<Hyperion::ComponentType> {
        std::size_t operator()(const Hyperion::ComponentType &type) const {
            return ((hash<bool>()(type.m_is_native_type) ^ (hash<Hyperion::Type>()(type.m_native_type) << 1)) >> 1) ^ (hash<void *>()(type.m_scripting_type) << 1);
        }
    };

}