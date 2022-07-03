//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/pipeline/forward/forward_render_lighting.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void ForwardRenderLighting::SetupLighting(RenderObjectContext &context, RenderCommandBuffer &command_buffer) {
    RenderObjectContextEnvironment environment = context.GetEnvironment();
    auto &lights = context.GetLights();
    RenderBuffer render_buffer_lighting = RenderBuffer(sizeof(ForwardLightingBuffer));
    ForwardLightingBuffer *lighting_buffer = reinterpret_cast<ForwardLightingBuffer *>(render_buffer_lighting.GetData().GetData());

    lighting_buffer->ambient_color = environment.ambient_light.intensity * environment.ambient_light.color;

    // Because of how the frame context gets populated, we know that the first light (if present and directional) is the main light.
    auto light_it = lights.begin();
    if (light_it != lights.end() && light_it->type == LightType::Directional) {
      const RenderObjectContextLight &main_light = *light_it;
      m_main_light = &main_light;
      CopyFrameLightToLight(main_light, lighting_buffer->main_light);
    } else {
      m_main_light = nullptr;
    }

    uint64 point_light_index = 0;
    for (const RenderObjectContextLight &light : lights) {
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
  Matrix4x4 ForwardRenderLighting::CalculateLightSpaceMatrixForMainLight() {
    if (m_main_light) {
      Vector3 light_position = Vector3(2.0f, 4.0f, 0.0f);
      Matrix4x4 light_view = Matrix4x4::LookAt(light_position, light_position + m_main_light->direction, Vector3::Up());
      Matrix4x4 light_projection = Matrix4x4::Orthographic(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
      Matrix4x4 light_space_matrix = light_projection * light_view;
      return light_space_matrix;
    } else {
      return Matrix4x4::Identity();
    }
  }

  //--------------------------------------------------------------
  void ForwardRenderLighting::CopyFrameLightToLight(const RenderObjectContextLight &frame_light, ForwardLight &light) {
    light.color = frame_light.color;
    light.intensity = Math::Clamp01(frame_light.intensity);
    light.direction = frame_light.direction;
    light.position = frame_light.position;
    light.range = frame_light.range;
    light.spot_inner_radius = frame_light.inner_spot_radius;
    light.spot_outer_radius = frame_light.outer_spot_radius;
    light.shadow_intensity = Math::Clamp01(frame_light.shadow_intensity);
    light.shadow_bias = frame_light.shadow_bias;
  }

}
