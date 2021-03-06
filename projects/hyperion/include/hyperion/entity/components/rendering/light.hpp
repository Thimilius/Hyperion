#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/math.hpp"
#include "hyperion/entity/components/behaviour.hpp"
#include "hyperion/rendering/data/light_data.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Light final : public Behaviour {
        HYP_REFLECT(Behaviour);
    public:
        inline void SetLightType(Rendering::LightType type) { m_data.type = type; }
        inline Rendering::LightType GetLightType() const { return m_data.type; }
        inline void SetIntensity(float32 intensity) { m_data.intensity = Math::Max(intensity, 0.0f); }
        inline float32 GetIntensity() const { return m_data.intensity; }
        inline void SetColor(Color color) { m_data.color = color; }
        inline Color GetColor() const { return m_data.color; }
        inline void SetRange(float32 range) { m_data.range = range; }
        inline float32 GetRange() const { return m_data.range; }
        inline void SetInnerSpotRadius(float32 inner_spot_radius) { m_data.inner_spot_radius = inner_spot_radius; }
        inline float32 GetInnerSpotRadius() const { return m_data.inner_spot_radius; }
        inline void SetOuterSpotRadius(float32 outer_spot_radius) { m_data.outer_spot_radius = outer_spot_radius; }
        inline float32 GetOuterSpotRadius() const { return m_data.outer_spot_radius; }

        Rendering::LightData GetData() const;
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        Light() : Behaviour("Light") { }
    private:
        static Light *Create();
    private:
        mutable Rendering::LightData m_data;
    };

}