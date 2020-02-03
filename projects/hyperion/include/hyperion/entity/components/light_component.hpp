#pragma once

#include "hyperion/entity/components/behaviour_component.hpp"
#include "hyperion/rendering/light.hpp"

namespace Hyperion {

    class LightComponent : public BehaviourComponent {
        HYP_OBJECT(LightComponent, BehaviourComponent);
    private:
        Rendering::Light light;
    public:
        LightComponent() : BehaviourComponent("LightComponent") { }

        inline Rendering::Light GetLight() const { return light; }

        inline void SetType(Rendering::LightType type) { light.type = type; }
        inline void SetIntensity(f32 intensity) { light.intensity = intensity; }
        inline void SetColor(Color color) { light.color = color; }
        inline void SetRange(f32 range) { light.range = range; }
        inline void SetInnerSpotRadius(f32 inner_spot_radius) { light.inner_spot_radius = inner_spot_radius; }
        inline void SetOuterSpotRadius(f32 outer_spot_radius) { light.outer_spot_radius = outer_spot_radius; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    };

}