#pragma once

#include "hyperion/core/math/vec2.hpp"
#include "hyperion/entity/components/component.hpp"

namespace Hyperion::UI {
    class Graphic;
}

namespace Hyperion::UI {

    class Canvas final : public Component {
        RTTR_ENABLE(Component);
    public:
        Canvas() : Component("Canvas") { }

        inline f32 GetScale() const {return m_scale; }
        inline void SetScale(f32 scale) { 
            m_scale = scale;
            UpdateScale();
        }
        inline Vec2 GetReferenceResoultion() const { return m_reference_resolution; }
        inline void SetReferenceResolution(Vec2 reference_resolution) {
            m_reference_resolution = reference_resolution;
            UpdateScale();
        }

        inline f32 GetFullScale() const { return m_full_scale; }

        inline const Vector<Graphic *> &GetGraphics() { return m_graphics; }
    protected:
        void OnCreate() override;
        void OnUpdate(f32 delta_time) override;
        void OnDestroy() override;
    private:
        void UpdateScale();

        void AddGraphic(Graphic *graphic);
        void RemoveGraphic(Graphic *graphic);
        void UpdateGraphicDepths();
    private:
        f32 m_scale = 1.0f;
        f32 m_full_scale = 1.0f;
        Vec2 m_reference_resolution = Vec2(1280, 720);

        f32 m_cached_display_width = 0;
        f32 m_cached_display_height = 0;

        Vector<Graphic *> m_graphics;
    private:
        friend class Hyperion::UI::Graphic;
    };

}