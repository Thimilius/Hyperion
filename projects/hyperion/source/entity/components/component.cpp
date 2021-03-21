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

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(Component)
    HYP_REFLECT_BASE(Object)
    HYP_REFLECT_END()

}
