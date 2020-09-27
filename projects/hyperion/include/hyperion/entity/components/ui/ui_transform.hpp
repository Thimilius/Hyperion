#pragma once

#include "hyperion/entity/components/transform.hpp"
#include "hyperion/core/math/vec2.hpp"

namespace Hyperion {
    class Entity;
}

namespace Hyperion {

    class UITransform : public Transform {
        RTTR_ENABLE(Transform);
    public:
        UITransform() : Transform("UITransform") { }

        Vec2 GetSize() const { return m_size; }
        void SetSize(Vec2 size) { m_size = size; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        Vec2 m_size;

        bool m_replace_on_destroy = true;
    private:
        friend class Hyperion::Entity;
    };

}