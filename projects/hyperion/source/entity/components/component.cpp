//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/component.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world_manager.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    ComponentType::ComponentType(const Type &native_type)
        : m_is_native_type(true), m_native_type(native_type), m_scripting_type(nullptr) { }

    //--------------------------------------------------------------
    ComponentType::ComponentType(Scripting::ScriptingType *scripting_type)
        : m_is_native_type(false), m_native_type(Type::get<Component>()), m_scripting_type(scripting_type) { }

    //--------------------------------------------------------------
    bool ComponentType::IsDerivedFrom(const ComponentType &base) const {
        if (m_is_native_type != base.m_is_native_type) {
            return false;
        }

        if (m_is_native_type) {
            return m_native_type.is_derived_from(base.m_native_type);
        } else {
            return m_scripting_type->IsDerivedFrom(base.m_scripting_type);
        }
    }

    //--------------------------------------------------------------
    void ComponentType::Serialize(ISerializationStream &stream) {
        stream.WriteBool("is_native_type", m_is_native_type);
        stream.WriteString("native_type_name", m_native_type.get_name().to_string());
    }

    //--------------------------------------------------------------
    void ComponentType::Deserialize(IDeserializationStream &stream, ReferenceContext &context) {
        m_is_native_type = stream.ReadBool("is_native_type");
        m_native_type = Type::get_by_name(stream.ReadString("native_type_name"));
        HYP_ASSERT(m_native_type.is_valid());
    }

    //--------------------------------------------------------------
    bool ComponentType::operator==(const ComponentType &other) const {
        if (m_is_native_type != other.m_is_native_type) {
            return false;
        } else {
            if (m_is_native_type) {
                return m_native_type == other.m_native_type;
            } else {
                return m_scripting_type == other.m_scripting_type;
            }
        }
    }

    //--------------------------------------------------------------
    bool ComponentType::operator!=(const ComponentType &other) const {
        return !(*this == other);
    }

    //--------------------------------------------------------------
    Transform *Component::GetTransform() const {
        return m_entity->GetTransform();
    }

    //--------------------------------------------------------------
    World *Component::GetWorld() const {
        return m_entity->GetWorld();
    }

    //--------------------------------------------------------------
    void Component::Serialize(ISerializationStream &stream) {
        Object::Serialize(stream);
    }

    //--------------------------------------------------------------
    void Component::Deserialize(IDeserializationStream &stream, ReferenceContext &context) {
        Object::Deserialize(stream, context);

        // The m_entity reference gets set by the entity itself.
    }

}
