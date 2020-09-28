#pragma once

#include "hyperion/core/math/vec2.hpp"
#include "hyperion/entity/components/component.hpp"

namespace Hyperion {

    class UICanvas : public Component {
        RTTR_ENABLE(Component);
    public:
        inline void SetScale(f32 scale) { m_scale = scale; }
        inline f32 GetScale() const { return m_scale; }
        inline void SetReferenceResolution(Vec2 reference_resolution) { m_reference_resolution = reference_resolution; }
        inline Vec2 GetReferenceResoultion() const { return m_reference_resolution; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        f32 m_scale = 1.0f;
        Vec2 m_reference_resolution = Vec2(1280, 720);
    };

}