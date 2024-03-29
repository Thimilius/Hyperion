//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/command/render_command_buffer.hpp"
#include "hyperion/render/object/render_object_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  struct ForwardLight {
    Color color;
    Vector3 direction;
    float32 intensity = 0.0f;
    Vector3 position;
    float32 range = 0.0f;
    float32 spot_inner_radius = 0.0f;
    float32 spot_outer_radius = 0.0f;
    float32 shadow_intensity = 0.0f;
    float32 shadow_bias = 0.0f;
  };

  struct ForwardLightingBuffer {
    Color ambient_color;

    ForwardLight main_light;

    ForwardLight point_lights[128];
  };

  class ForwardRenderLighting {
  public:
    inline const RenderObjectContextLight *GetMainLight() const { return m_main_light; } 
    
    void SetupLighting(RenderObjectContext &context, RenderCommandBuffer &command_buffer);
    Matrix4x4 CalculateLightSpaceMatrixForMainLight();
  private:
    static void CopyFrameLightToLight(const RenderObjectContextLight &frame_light, ForwardLight &light);
  private:
    const RenderObjectContextLight *m_main_light = nullptr;
  };

}
