//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/pipelines/forward/forward_render_lighting.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void ForwardRenderLighting::SetupLighting(RenderFrameContext &context, RenderCommandBuffer &command_buffer) {
    RenderFrameContextEnvironment environment = context.GetEnvironment();
    auto &lights = context.GetLights();
    RenderBuffer render_buffer_lighting = RenderBuffer(sizeof(ForwardLightingBuffer));
    ForwardLightingBuffer *lighting_buffer = reinterpret_cast<ForwardLightingBuffer *>(render_buffer_lighting.GetData().GetData());

    lighting_buffer->ambient_color = environment.ambient_light.intensity * environment.ambient_light.color;

    // Because of how the frame context gets populated, we know that the first light (if present and directional) is the main light.
    auto light_it = lights.begin();
    if (light_it != lights.end() && light_it->type == LightType::Directional) {
      const RenderFrameContextLight &main_light = *light_it;
      CopyFrameLightToLight(main_light, lighting_buffer->main_light);
    }

    uint64 point_light_index = 0;
    for (const RenderFrameContextLight &light : lights) {
      if (light.type == LightType::Point) {
        // HACK: For now we just ignore every light that is beyond our hardcoded limit.

        if (point_light_index < 128) {
          CopyFrameLightToLight(light, lighting_buffer->point_lights[point_light_index++]);
        }
      }
    }

    command_buffer.SetGlobalBuffer(0, std::move(render_buffer_lighting));
  }

  //--------------------------------------------------------------
  void ForwardRenderLighting::CopyFrameLightToLight(const RenderFrameContextLight &frame_light, ForwardLight &light) {
    light.color = frame_light.color;
    light.intensity = frame_light.intensity;
    light.direction = frame_light.direction;
    light.position = frame_light.position;
    light.range = frame_light.range;
    light.spot_inner_radius = frame_light.inner_spot_radius;
    light.spot_outer_radius = frame_light.outer_spot_radius;
  }

}
