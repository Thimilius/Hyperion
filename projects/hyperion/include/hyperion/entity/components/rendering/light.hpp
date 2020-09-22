#pragma once

#include "hyperion/core/color.hpp"
#include "hyperion/entity/components/behaviour.hpp"

namespace Hyperion {

    enum class LightType {
        Directional,
        Point,
        Spot
    };

    class Light : public Behaviour {
        HYP_OBJECT(Light, Behaviour);
    public:
        Light() : Behaviour("Light") { }

        inline void SetLightType(LightType type) { m_type = type; }
        inline LightType GetLightType() const { return m_type; }
        inline void SetIntensity(f32 intensity) { m_intensity = Math::Max(intensity, 0.0f); }
        inline f32 GetIntensity() const { return m_intensity; }
        inline void SetColor(Color color) { m_color = color; }
        inline Color GetColor() const { return m_color; }
        inline void SetRange(f32 range) { m_range = range; }
        inline f32 GetRange() const { return m_range; }
        inline void SetInnerSpotRadius(f32 inner_spot_radius) { m_inner_spot_radius = inner_spot_radius; }
        inline f32 GetInnerSpotRadius() const { return m_inner_spot_radius; }
        inline void SetOuterSpotRadius(f32 outer_spot_radius) { m_outer_spot_radius = outer_spot_radius; }
        inline f32 GetOuterSpotRadius() const { return m_outer_spot_radius; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        LightType m_type = LightType::Point;

        Color m_color = Color::White();
        f32 m_intensity = 1.0f;

        f32 m_range = 10.0f;

        f32 m_inner_spot_radius = 1.0f;
        f32 m_outer_spot_radius = 1.0f;
    };

}