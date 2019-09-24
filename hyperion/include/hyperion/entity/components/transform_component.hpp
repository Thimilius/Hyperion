#pragma once

#include "hyperion/math/math.hpp"
#include "hyperion/entity/components/component.hpp"

namespace Hyperion::Entity {

    class CTransformComponent : public CComponent {
    private:
        Math::SVec3 m_Position;
        Math::SVec3 m_Rotation;
        Math::SVec3 m_Scale = Math::SVec3::One();
    public:
        inline Math::SVec3 GetPosition() const { return m_Position; }
        void SetPosition(const Math::SVec3 position) { m_Position = position; }

        inline Math::SVec3 GetRotation() const { return m_Rotation; }
        void SetRotation(const Math::SVec3 rotation) { m_Rotation = rotation; }
        
        inline Math::SVec3 GetScale() const { return m_Scale; }
        void SetScale(const Math::SVec3 scale) { m_Scale = scale; }

        Math::SMat4 GetLocalToWorldMatrix() const;

        static SComponentType *GetStaticType() {
            static SComponentType type({ "TransformComponent" });
            return &type;
        }

        inline virtual SComponentType *GetType() const override { return GetStaticType(); }
    };

}