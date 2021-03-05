//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/behaviour.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/transform.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    bool Behaviour::IsActiveAndEnabled() {
        return m_enabled && GetTransform()->GetEntity()->IsActiveInHierarchy();
    }

    //--------------------------------------------------------------
    void Behaviour::Serialize(ISerializationStream &stream) {
        Component::Serialize(stream);

        stream.WriteBool("enabled", m_enabled);
    }

    //--------------------------------------------------------------
    void Behaviour::Deserialize(IDeserializationStream &stream, ReferenceContext &context) {
        Component::Deserialize(stream, context);

        m_enabled = stream.ReadBool("enabled");
    }

}
