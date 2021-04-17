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
    void Component::Deserialize(IDeserializationStream &stream) {
        Object::Deserialize(stream);

        // The m_entity reference gets set by the entity itself.
    }

    //--------------------------------------------------------------
    void Component::OnDestroy() {
        auto range = m_entity->m_components.equal_range(GetType());
        for (auto it = range.first; it != range.second; ) {
            if (it->second == this) {
                it = m_entity->m_components.erase(it);
            } else {
                it++;
            }
        }

        DisableUpdate();
    }

    //--------------------------------------------------------------
    void Component::EnableUpdate() {
        m_entity->GetWorld()->RegisterComponentForUpdate(this);
    }

    //--------------------------------------------------------------
    void Component::DisableUpdate() {
        m_entity->GetWorld()->UnregisterComponentForUpdate(this);
    }

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(Component)
    HYP_REFLECT_BASE(Object)
    HYP_REFLECT_END()

}
