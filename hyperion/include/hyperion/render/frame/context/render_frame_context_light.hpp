//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/render/types/render_types_light.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  struct RenderFrameContextLight {
    LightType type = LightType::Directional;

    Color color;
    float32 intensity = 1.0f;
    
    LightShadows shadows = LightShadows::None;
    float32 shadow_intensity = 1.0f;
    float32 shadow_bias = 0.005f;

    Vector3 direction;
    Vector3 position;

    float32 range = 10.0f;

    float32 inner_spot_radius = 1.0f;
    float32 outer_spot_radius = 1.0f;
  };

}
