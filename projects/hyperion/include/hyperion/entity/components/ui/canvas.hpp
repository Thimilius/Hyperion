 #pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/entity/components/component.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Canvas : public Component {
        HYP_REFLECT(Component);
    public:
        inline float32 GetScale() const { return m_scale; }
        inline float32 GetScaleFactor() const { return m_scale_factor; }
        inline void SetScaleFactor(float32 scale_factor) {
            m_scale_factor = scale_factor;
            UpdateScale();
        }
        inline Vec2 GetReferenceResoultion() const { return m_reference_resolution; }
        inline void SetReferenceResolution(Vec2 reference_resolution) {
            m_reference_resolution = reference_resolution;
            UpdateScale();
        }

        void UpdateScale();
    protected:
        void OnCreate() override;
    private:
        Canvas() : Component("Canvas") { }
    private:
        static Canvas *Create();
    private:
        float32 m_scale = 1.0f;
        float32 m_scale_factor = 1.0f;
        Vec2 m_reference_resolution = Vec2(1280, 720);

        uint32 m_cached_display_width;
        uint32 m_cached_display_height;
    };

}