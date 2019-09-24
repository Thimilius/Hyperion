#include "hyppch.hpp"

#include "hyperion/entity/components/transform_component.hpp"

namespace Hyperion::Entity {

    using namespace Math;

    Math::SMat4 CTransformComponent::GetLocalToWorldMatrix() const {
        SMat4 translation = SMat4::Translate(m_Position);
        SMat4 rotX = SMat4::Rotate(SVec3(1, 0, 0), m_Rotation.x);
        SMat4 rotY = SMat4::Rotate(SVec3(0, 1, 0), m_Rotation.y);
        SMat4 rotZ = SMat4::Rotate(SVec3(0, 0, 1), m_Rotation.z);
        SMat4 rotation = rotX * rotY * rotZ;
        SMat4 scale = SMat4::Scale(m_Scale);
        return translation * rotation * scale;
    }

}