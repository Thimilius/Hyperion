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

  class ForwardRenderLighting final {
  public:
    static void SetupLighting(RenderFrameContext &context, RenderCommandBuffer &command_buffer);
  private:
    ForwardRenderLighting() = delete;
    ~ForwardRenderLighting() = delete;
  private:
    static void CopyFrameLightToLight(const RenderFrameContextLight &frame_light, ForwardLight &light);
  };

}
