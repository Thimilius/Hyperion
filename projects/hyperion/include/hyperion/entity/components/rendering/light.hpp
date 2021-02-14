#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/math.hpp"
#include "hyperion/entity/components/behaviour.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class LightType {
        Directional,
        Point,
        Spot
    };

    class Light final : public Behaviour {
        HYP_REFLECT(Behaviour);
    public:
        inline void SetLightType(LightType type) { m_type = type; }
        inline LightType GetLightType() const { return m_type; }
        inline void SetIntensity(float32 intensity) { m_intensity = Math::Max(intensity, 0.0f); }
        inline float32 GetIntensity() const { return m_intensity; }
        inline void SetColor(Color color) { m_color = color; }
        inline Color GetColor() const { return m_color; }
        inline void SetRange(float32 range) { m_range = range; }
        inline float32 GetRange() const { return m_range; }
        inline void SetInnerSpotRadius(float32 inner_spot_radius) { m_inner_spot_radius = inner_spot_radius; }
        inline float32 GetInnerSpotRadius() const { return m_inner_spot_radius; }
        inline void SetOuterSpotRadius(float32 outer_spot_radius) { m_outer_spot_radius = outer_spot_radius; }
        inline float32 GetOuterSpotRadius() const { return m_outer_spot_radius; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        Light() : Behaviour("Light") { }
    private:
        static Light *Create();
    private:
        LightType m_type = LightType::Point;

        Color m_color = Color::White();
        float32 m_intensity = 1.0f;

        float32 m_range = 10.0f;

        float32 m_inner_spot_radius = 1.0f;
        float32 m_outer_spot_radius = 1.0f;
    };

}