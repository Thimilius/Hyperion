//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  struct ForwardLight {
    Color color;
    Vector3 direction;
    float32 intensity;
    Vector3 position;
    float32 range;
    float32 spot_inner_radius;
    float32 spot_outer_radius;

    float32 padding[2];
  };

  struct ForwardLightingBuffer {
    Color ambient_color;

    ForwardLight main_light;

    ForwardLight point_lights[128];
  };

  class ForwardRenderLighting {
  public:
    inline const RenderFrameContextLight *GetMainLight() const { return m_main_light; } 
    
    void SetupLighting(RenderFrameContext &context, RenderCommandBuffer &command_buffer);
    Matrix4x4 CalculateLightSpaceMatrixForMainLight();
  private:
    static void CopyFrameLightToLight(const RenderFrameContextLight &frame_light, ForwardLight &light);
  private:
    const RenderFrameContextLight *m_main_light = nullptr;
  };

}
