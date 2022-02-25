//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/opengl/opengl_render_driver.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/render/driver/opengl/opengl_debug_group.hpp"
#include "hyperion/render/driver/opengl/opengl_shader_compiler.hpp"
#include "hyperion/render/driver/opengl/opengl_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void OpenGLRenderDriver::Initialize() {
    {
      glEnable(GL_DEPTH_TEST);
      glFrontFace(GL_CW);
    }

    m_storage.Initialize();
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::HandleAssets(RenderAssetContext &asset_context) {
    m_storage.LoadAssets(asset_context);
    m_storage.UnloadAssets(asset_context);
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::Render(RenderFrame *render_frame) {
    ExecuteRenderFrame(render_frame);
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::Shutdown() {
    // NOTE: We could bother unloading every asset on the GPU but that is pretty much just a waste of time.
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::ExecuteRenderFrame(RenderFrame *render_frame) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.ExecuteRenderFrameCommands");

    const RenderFrameContext &render_frame_context = render_frame->GetContext();

    const Array<RenderFrameCommand> &frame_commands = render_frame->GetCommands();
    for (const RenderFrameCommand &frame_command : frame_commands) {
      switch (frame_command.type) {
        case RenderFrameCommandType::SetCamera:
        {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.SetCamera");
          OpenGLDebugGroup debug_group("SetCamera");

          const RenderFrameCommandSetCamera &set_camera = std::get<RenderFrameCommandSetCamera>(frame_command.data);
          m_static.camera_index = set_camera.camera_index;

          const RenderFrameContextCamera &render_frame_context_camera = render_frame_context.GetCameras()[m_static.camera_index];

          OpenGLUniformBufferCamera uniform_buffer_camera;
          uniform_buffer_camera.camera_view_matrix = render_frame_context_camera.view_matrix;
          uniform_buffer_camera.camera_projection_matrix = render_frame_context_camera.projection_matrix;
          glNamedBufferSubData(m_storage.GetStatic().camera_uniform_buffer, 0, sizeof(OpenGLUniformBufferCamera), &uniform_buffer_camera);

          const CameraViewport &viewport = render_frame_context_camera.viewport;
          glViewport(viewport.x, viewport.y, viewport.width, viewport.height);

          break;
        }
        case RenderFrameCommandType::ExecuteCommandBuffer:
        {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.ExecuteCommandBuffer");
          OpenGLDebugGroup debug_group("ExecuteCommandBuffer");

          const RenderFrameCommandExecuteCommandBuffer &execute_command_buffer = std::get<RenderFrameCommandExecuteCommandBuffer>(frame_command.data);

          ExecuteCommandBuffer(render_frame, execute_command_buffer.command_buffer);

          break;
        }
        case RenderFrameCommandType::DrawMeshes:
        {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.DrawMeshes");
          OpenGLDebugGroup debug_group("DrawMeshes");

          const RenderFrameCommandDrawMeshes &draw_meshes = std::get<RenderFrameCommandDrawMeshes>(frame_command.data);

          PrepareObjects(render_frame, draw_meshes.sorted_objects, draw_meshes.drawing_parameters);
          DrawMeshes(render_frame_context.GetEnvironment(), render_frame_context.GetLights(), draw_meshes.drawing_parameters);

          break;
        }
        case RenderFrameCommandType::DrawUI:
        {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.DrawUI");
          OpenGLDebugGroup debug_group("DrawUI");

          DrawUI(render_frame_context.GetUIObjects());

          break;
        }
        case RenderFrameCommandType::DrawObjectIds:
        {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.DrawObjectIds");
          OpenGLDebugGroup debug_group("DrawObjectIds");

          const RenderFrameCommandDrawObjectIds &draw_object_ids = std::get<RenderFrameCommandDrawObjectIds>(frame_command.data);

          DrawObjectIds(render_frame_context, draw_object_ids.render_target_id);

          break;
        }
        case RenderFrameCommandType::DrawGizmos:
        {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.DrawGizmos");
          OpenGLDebugGroup debug_group("DrawGizmos");

          const RenderFrameCommandDrawGizmos &draw_gizmos = std::get<RenderFrameCommandDrawGizmos>(frame_command.data);

          glDepthMask(GL_FALSE);
          glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

          const OpenGLShader &opengl_shader = m_storage.GetShader(draw_gizmos.shader_id);
          glUseProgram(opengl_shader.program);

          GLint model_location = glGetUniformLocation(opengl_shader.program, "u_model");

          if (draw_gizmos.grid.should_draw) {
            glProgramUniformMatrix4fv(opengl_shader.program, model_location, 1, GL_FALSE, draw_gizmos.grid.local_to_world.elements);

            const OpenGLMesh &opengl_mesh = m_storage.GetMesh(draw_gizmos.grid.mesh_id);
            UseMesh(opengl_mesh);

            SubMesh sub_mesh = opengl_mesh.sub_meshes[0];
            DrawSubMesh(sub_mesh);
          }

          if (draw_gizmos.should_draw_all_bounds) {
            glProgramUniformMatrix4fv(opengl_shader.program, model_location, 1, GL_FALSE, Matrix4x4::Identity().elements);

            for (const RenderFrameContextObjectMesh &object : render_frame_context.GetMeshObjects()) {
              DrawRenderBounds(object.bounds);
            }
          }

          break;
        }
        case RenderFrameCommandType::DrawEditorUI:
        {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.DrawEditorUI");
          OpenGLDebugGroup debug_group("DrawEditorUI");

          DrawUI(render_frame_context.GetEditorUIObjects());

          break;
        }
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
      }
    }
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::ExecuteCommandBuffer(RenderFrame *render_frame, const RenderCommandBuffer &command_buffer) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.ExecuteCommandBuffer");

    const Array<RenderCommandBufferCommand> &buffer_commands = command_buffer.GetCommands();
    for (const RenderCommandBufferCommand &buffer_command : buffer_commands) {
      switch (buffer_command.type) {
        case RenderCommandBufferCommandType::ClearRenderTarget:
        {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommandBufferCommand.Clear");
          OpenGLDebugGroup debug_group("ClearRenderTarget");

          const RenderCommandBufferCommandClearRenderTarget &clear_render_target = std::get<RenderCommandBufferCommandClearRenderTarget>(buffer_command.data);

          Color background_color = clear_render_target.color;
          glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);

          // We have to make sure that we can clear the depth buffer by enabling the depth mask.
          glDepthMask(GL_TRUE);
          glClear(OpenGLUtilities::GetClearFlags(clear_render_target.flags));
          break;
        }
        case RenderCommandBufferCommandType::SetRenderTarget:
        {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommandBufferCommand.SetRenderTarget");
          OpenGLDebugGroup debug_group("SetRenderTarget");

          const RenderCommandBufferCommandSetRenderTarget &set_render_target = std::get<RenderCommandBufferCommandSetRenderTarget>(buffer_command.data);

          UseRenderTexture(set_render_target.id);

          break;
        }
        case RenderCommandBufferCommandType::Blit:
        {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommandBufferCommand.Blit");
          OpenGLDebugGroup debug_group("Blit");

          const RenderCommandBufferCommandBlit &blit = std::get<RenderCommandBufferCommandBlit>(buffer_command.data);

          GLint source_width = Display::GetWidth();
          GLint source_height = Display::GetHeight();
          GLuint source_framebuffer = 0;
          GLuint source_color_attachment = 0;
          if (blit.source.id != RenderTargetId::Default().id) {
            const OpenGLRenderTexture &opengl_render_texture = m_storage.GetRenderTexture(blit.source.id);
            source_width = opengl_render_texture.width;
            source_height = opengl_render_texture.height;
            source_framebuffer = opengl_render_texture.framebuffer;

            // TODO: Use correct attachment.
            source_color_attachment = opengl_render_texture.attachments.Get(0).attachment;
          }

          GLint destination_width = Display::GetWidth();
          GLint destination_height = Display::GetHeight();
          GLuint destination_framebuffer = 0;
          if (blit.destination.id != RenderTargetId::Default().id) {
            const OpenGLRenderTexture &opengl_render_texture = m_storage.GetRenderTexture(blit.destination.id);
            destination_width = opengl_render_texture.width;
            destination_height = opengl_render_texture.height;
            destination_framebuffer = opengl_render_texture.framebuffer;
          }

          // Catch the case where we copy from the default back buffer.
          if (source_framebuffer == 0) {
            GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;

            glBlitNamedFramebuffer(
              source_framebuffer,
              destination_framebuffer,
              0,
              0,
              source_width,
              source_height,
              0,
              0,
              destination_width,
              destination_height,
              mask,
              GL_NEAREST);
          } else {
            GLuint current_framebuffer = 0;
            glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, reinterpret_cast<GLint *>(&current_framebuffer));

            glBindFramebuffer(GL_FRAMEBUFFER, destination_framebuffer);

            glDisable(GL_BLEND);

            glUseProgram(m_storage.GetStatic().fullscreen_shader);
            glBindTextureUnit(0, source_color_attachment);
            glBindVertexArray(m_storage.GetStatic().fullscreen_vertex_array);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glBindFramebuffer(GL_FRAMEBUFFER, current_framebuffer);
          }

          break;
        }
        case RenderCommandBufferCommandType::SetGlobalBuffer:
        {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommandBufferCommand.SetGlobalBuffer");
          OpenGLDebugGroup debug_group("SetGlobalBuffer");

          const RenderCommandBufferCommandSetGlobalBuffer &set_global_buffer = std::get<RenderCommandBufferCommandSetGlobalBuffer>(buffer_command.data);
          auto &data = set_global_buffer.render_buffer.GetData();

          // HACK: This is kind of stupid.
          // We need something more sophisticated which abstracts the proper shader uniform names and binding points.
          GLuint buffer_id = -1;
          if (set_global_buffer.id == 0) {
            buffer_id = m_static.lighting_uniform_buffer;

            if (buffer_id == -1) {
              glCreateBuffers(1, &buffer_id);
              glNamedBufferData(buffer_id, data.GetLength(), data.GetData(), GL_DYNAMIC_DRAW);
              glBindBufferBase(GL_UNIFORM_BUFFER, 1, buffer_id);

              m_static.lighting_uniform_buffer = buffer_id;

              return;
            }
          }

          glNamedBufferSubData(buffer_id, 0, data.GetLength(), data.GetData());
          break;
        }
        case RenderCommandBufferCommandType::RequestAsyncReadback:
        {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommandBufferCommand.RequestAsyncReadback");
          OpenGLDebugGroup debug_group("RequestAsyncReadback");

          const RenderCommandBufferCommandRequestAsyncReadback &request_async_readback = std::get<RenderCommandBufferCommandRequestAsyncReadback>(buffer_command.data);

          auto render_texture_it = m_storage.FindRenderTexture(request_async_readback.render_target_id.id);
          if (render_texture_it != m_storage.GetRenderTextureEnd()) {
            AsyncRequestResult &async_request_result = render_frame->AddAsyncRequestResult();
            async_request_result.callback = request_async_readback.callback;
            async_request_result.result.region = request_async_readback.region;

            const OpenGLRenderTexture &render_texture = render_texture_it->second;
            const OpenGLRenderTextureAttachment &attachment = render_texture.attachments[request_async_readback.attachment_index];
            RenderTextureFormat format = attachment.format;
            HYP_ASSERT(format != RenderTextureFormat::Depth24Stencil8);

            // Make sure we are not out of bounds when accessing the render texture.
            RectInt region = request_async_readback.region;
            bool8 x_range_is_not_valid = region.x < 0 || (region.width + region.x) < 0 || (region.width + region.x) > static_cast<int32>(render_texture.width);
            bool8 y_range_is_not_valid = region.y < 0 || (region.height + region.y) < 0 || (region.height + region.y) > static_cast<int32>(render_texture.height);
            if (x_range_is_not_valid || y_range_is_not_valid) {
              HYP_LOG_ERROR("OpenGL", "Trying to read out-of-bounds data of a render texture!");
            } else {
              // We want to make sure that the buffer we got passed does actually have enough space.
              GLsizei buffer_size = OpenGLUtilities::GetRenderTextureBufferSize(region, format);
              async_request_result.result.data.Resize(buffer_size);

              GLenum format_value = OpenGLUtilities::GetRenderTextureFormat(format);
              GLenum format_type = OpenGLUtilities::GetRenderTextureFormatType(format);

              glBindFramebuffer(GL_READ_FRAMEBUFFER, render_texture.framebuffer);
              glNamedFramebufferReadBuffer(render_texture.framebuffer, GL_COLOR_ATTACHMENT0);
              glReadnPixels(region.x, region.y, region.width, region.height, format_value, format_type, buffer_size, async_request_result.result.data.GetData());
            }
          }
          break;
        }
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
      }
    }
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::PrepareObjects(RenderFrame *render_frame, const Array<uint32> &sorted_objects, DrawingParametes drawing_parameters) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.PrepareObjects");

    Array<GroupedShader> grouped_shaders;

    const RenderFrameContext &render_frame_context = render_frame->GetContext();
    const Array<RenderFrameContextObjectMesh> &mesh_objects = render_frame_context.GetMeshObjects();

    for (uint32 index : sorted_objects) {
      const RenderFrameContextObjectMesh &render_frame_context_object_mesh = mesh_objects[index];

      AssetId shader_id = render_frame_context_object_mesh.shader_id;
      AssetId material_id = render_frame_context_object_mesh.material_id;
      AssetId mesh_id = render_frame_context_object_mesh.mesh_id;

      if ((render_frame_context_object_mesh.layer_mask & drawing_parameters.filter_mask) != render_frame_context_object_mesh.layer_mask) {
        continue;
      }

      auto opengl_shader_it = m_storage.FindShader(shader_id);
      if (opengl_shader_it == m_storage.GetShaderEnd()) {
        HYP_LOG_ERROR("OpenGL", "Failed to retrieve OpenGL shader!");
        continue;
      } else {
        ShaderRenderOrder render_order = opengl_shader_it->second.attributes.render_order;
        if (render_order != drawing_parameters.render_order) {
          continue;
        }
      }

      auto shaders_it = std::find_if(grouped_shaders.begin(), grouped_shaders.end(), [shader_id](const GroupedShader &grouped_shader) {
        return grouped_shader.shader->id == shader_id;
      });
      GroupedShader *grouped_shader = nullptr;
      if (shaders_it == grouped_shaders.end()) {
        GroupedShader new_grouped_shader;
        new_grouped_shader.shader = &m_storage.GetShader(shader_id);
        grouped_shaders.Add(new_grouped_shader);
        grouped_shader = &grouped_shaders.GetLast();
      } else {
        grouped_shader = &*shaders_it;
      }

      Map<AssetId, GroupedMaterial> &materials = grouped_shader->materials;
      auto materials_it = materials.Find(material_id);
      GroupedMaterial *grouped_material = nullptr;
      if (materials_it == materials.end()) {
        GroupedMaterial new_grouped_material;
        new_grouped_material.material = &m_storage.GetMaterial(material_id);
        materials.Insert(material_id, new_grouped_material);
        grouped_material = &materials.Get(material_id);
      } else {
        grouped_material = &materials.Get(material_id);
      }

      Map<AssetId, GroupedMesh> &meshes = grouped_material->meshes;
      auto meshes_it = meshes.Find(mesh_id);
      GroupedMesh *grouped_mesh = nullptr;
      if (meshes_it == meshes.end()) {
        GroupedMesh new_grouped_material;
        new_grouped_material.mesh = &m_storage.GetMesh(mesh_id);
        meshes.Insert(mesh_id, new_grouped_material);
        grouped_mesh = &meshes.Get(mesh_id);
      } else {
        grouped_mesh = &meshes.Get(mesh_id);
      }

      GroupedObject grouped_object;
      grouped_object.local_to_world = render_frame_context_object_mesh.local_to_world;
      grouped_object.sub_mesh_index = render_frame_context_object_mesh.sub_mesh_index;

      // TODO: Can we make this check here at compile time and divert the runtime check to the call site of GroupObjects?
      // This would save this branch check on every iteration.
      if ((drawing_parameters.per_object_data & PerObjectData::LightIndices) == PerObjectData::LightIndices) {
        SetupPerObjectLightIndices(render_frame_context, grouped_object, render_frame_context_object_mesh.position);
      }

      grouped_mesh->objects.Add(grouped_object);
    }

    {
      HYP_PROFILE_SCOPE("OpenGLRenderDriver.MoveGroupedShaders");

      m_grouped_shaders = std::move(grouped_shaders);
    }
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::SetupPerObjectLightIndices(const RenderFrameContext &render_frame_context, GroupedObject &grouped_object, Vector3 object_position) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.SetupPerObjectLightIndices");

    // HACK: Currently this is pretty hardcoded for 4 light indices per object (and a maximum of 128 global lights).

    grouped_object.light_count = 0;
    grouped_object.light_indices[0] = UINT32_MAX;
    grouped_object.light_indices[1] = UINT32_MAX;
    grouped_object.light_indices[2] = UINT32_MAX;
    grouped_object.light_indices[3] = UINT32_MAX;

    uint32 light_count = 0;
    uint32 light_index = 0;
    float32 smallest_distances[4] = { FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX };

    // TODO: Add culling for lights.
    for (const RenderFrameContextLight &light : render_frame_context.GetLights()) {
      if (light.type == LightType::Point && light_index < 128) {
        float32 distance_to_light = (light.position - object_position).SqrMagnitude();

        // Is the light even in range?
        if (distance_to_light <= (light.range * light.range)) {
          if (distance_to_light < smallest_distances[0]) {
            smallest_distances[0] = distance_to_light;

            grouped_object.light_indices[3] = grouped_object.light_indices[2];
            grouped_object.light_indices[2] = grouped_object.light_indices[1];
            grouped_object.light_indices[1] = grouped_object.light_indices[0];
            grouped_object.light_indices[0] = light_index;
          } else if (distance_to_light < smallest_distances[1]) {
            smallest_distances[1] = distance_to_light;

            grouped_object.light_indices[3] = grouped_object.light_indices[2];
            grouped_object.light_indices[2] = grouped_object.light_indices[1];
            grouped_object.light_indices[1] = light_index;
          } else if (distance_to_light < smallest_distances[2]) {
            smallest_distances[2] = distance_to_light;

            grouped_object.light_indices[3] = grouped_object.light_indices[2];
            grouped_object.light_indices[2] = light_index;
          } else if (distance_to_light < smallest_distances[3]) {
            smallest_distances[3] = distance_to_light;

            grouped_object.light_indices[3] = light_index;
          }
        }

        light_index++;
      }
    }

    for (uint64 i = 0; i < 4; i++) {
      if (grouped_object.light_indices[i] != UINT32_MAX) {
        grouped_object.light_count++;
      }
    }
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::DrawMeshes(const RenderFrameContextEnvironment &environment, const Array<RenderFrameContextLight> &lights, DrawingParametes drawing_parameters) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.DrawMeshes");

    for (const GroupedShader &grouped_shader : m_grouped_shaders) {
      HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderGroupedShader");

      const OpenGLShader &opengl_shader = *grouped_shader.shader;
      UseShader(opengl_shader);

      for (const auto &[material_id, grouped_material] : grouped_shader.materials) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderGroupedMaterial");

        UseMaterial(opengl_shader, *grouped_material.material);

        for (const auto &[mesh_id, grouped_mesh] : grouped_material.meshes) {
          HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderGroupedMesh");
          OpenGLDebugGroup debug_group("DrawMesh");

          const OpenGLMesh &opengl_mesh = *grouped_mesh.mesh;
          UseMesh(opengl_mesh);

          GLint model_location = glGetUniformLocation(opengl_shader.program, "u_model");
          GLint light_count_location = glGetUniformLocation(opengl_shader.program, "u_light_count");
          GLint light_indices_location = glGetUniformLocation(opengl_shader.program, "u_light_indices");
          for (const GroupedObject &object : grouped_mesh.objects) {
            HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameMeshObject");

            glProgramUniformMatrix4fv(opengl_shader.program, model_location, 1, GL_FALSE, object.local_to_world.elements);
            if ((drawing_parameters.per_object_data & PerObjectData::LightIndices) == PerObjectData::LightIndices) {
              glProgramUniform1ui(opengl_shader.program, light_count_location, object.light_count);
              glProgramUniform4uiv(opengl_shader.program, light_indices_location, 1, object.light_indices);
            }

            SubMesh sub_mesh = opengl_mesh.sub_meshes[object.sub_mesh_index];
            DrawSubMesh(sub_mesh);
          }
        }
      }
    }
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::DrawSubMesh(const SubMesh &sub_mesh) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.DrawSubMesh");

    void *index_offset = reinterpret_cast<void *>(static_cast<uint32>(sub_mesh.index_offset) * sizeof(uint32));

    m_stats.draw_calls += 1;
    m_stats.vertex_count += sub_mesh.vertex_count;
    m_stats.triangle_count += sub_mesh.index_count;

    GLenum topology = OpenGLUtilities::GetTopology(sub_mesh.topology);
    glDrawElementsBaseVertex(topology, sub_mesh.index_count, GL_UNSIGNED_INT, index_offset, sub_mesh.vertex_offset);
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::DrawUI(const Array<RenderFrameContextObjectUI> &elements) {
    {
      uint32 width = Display::GetWidth();
      uint32 height = Display::GetHeight();
      float32 half_width = static_cast<float32>(width) / 2.0f;
      float32 half_height = static_cast<float32>(height) / 2.0f;

      OpenGLUniformBufferCamera uniform_buffer_camera;
      uniform_buffer_camera.camera_view_matrix = Matrix4x4::Identity();
      uniform_buffer_camera.camera_projection_matrix = Matrix4x4::Orthographic(-half_width, half_width, -half_height, half_height, -1.0f, 1.0f);
      glNamedBufferSubData(m_storage.GetStatic().camera_uniform_buffer, 0, sizeof(OpenGLUniformBufferCamera), &uniform_buffer_camera);

      glViewport(0, 0, width, height);
    }

    for (const RenderFrameContextObjectUI &element : elements) {
      const OpenGLShader &opengl_shader = m_storage.GetShader(element.shader_id);
      UseShader(opengl_shader);

      const OpenGLMaterial &opengl_material = m_storage.GetMaterial(element.material_id);
      UseMaterial(opengl_shader, opengl_material);
      if (!element.enable_blending) {
        glDisable(GL_BLEND);
      }

      OpenGLDebugGroup debug_group("DrawMesh");
      {
        GLint model_location = glGetUniformLocation(opengl_shader.program, "u_model");
        if (model_location >= 0) {
          glProgramUniformMatrix4fv(opengl_shader.program, model_location, 1, GL_FALSE, element.local_to_world.elements);
        }

        GLint color_location = glGetUniformLocation(opengl_shader.program, "u_color");
        if (color_location >= 0) {
          Color color = element.color;
          glProgramUniform4f(opengl_shader.program, color_location, color.r, color.g, color.b, color.a);
        }

        if (element.texture.id != AssetInfo::INVALID_ID) {
          GLint texture_location = glGetUniformLocation(opengl_shader.program, "u_texture");
          if (texture_location >= 0) {
            SetMaterialTextureProperty(element.texture, 0, opengl_shader.program, texture_location);
          }
        }
      }

      const OpenGLMesh &opengl_mesh = m_storage.GetMesh(element.mesh_id);
      UseMesh(opengl_mesh);

      DrawSubMesh(opengl_mesh.sub_meshes[0]);
    }
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::DrawObjectIds(const RenderFrameContext &render_frame_context, RenderTargetId render_target_id) {
    {
      const RenderFrameContextCamera &render_frame_context_camera = render_frame_context.GetCameras()[m_static.camera_index];

      OpenGLUniformBufferCamera uniform_buffer_camera;
      uniform_buffer_camera.camera_view_matrix = render_frame_context_camera.view_matrix;
      uniform_buffer_camera.camera_projection_matrix = render_frame_context_camera.projection_matrix;
      glNamedBufferSubData(m_storage.GetStatic().camera_uniform_buffer, 0, sizeof(OpenGLUniformBufferCamera), &uniform_buffer_camera);
    }

    GLuint framebuffer = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, reinterpret_cast<GLint *>(&framebuffer));

    UseRenderTexture(render_target_id);
    const OpenGLRenderTexture &opengl_render_texture = m_storage.GetRenderTexture(render_target_id.id);
    glDepthMask(GL_TRUE);
    GLuint clear_value = UINT32_MAX;
    glClearNamedFramebufferuiv(opengl_render_texture.framebuffer, GL_COLOR, GL_NONE, &clear_value);
    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    const OpenGLShader &opengl_shader = m_storage.GetStatic().object_id_shader;
    UseShader(opengl_shader);

    for (const RenderFrameContextObjectMesh &mesh_object : render_frame_context.GetMeshObjects()) {
      OpenGLDebugGroup debug_group("DrawMesh");
      const OpenGLMesh &opengl_mesh = m_storage.GetMesh(mesh_object.mesh_id);

      GLint model_location = glGetUniformLocation(opengl_shader.program, "u_model");
      glProgramUniformMatrix4fv(opengl_shader.program, model_location, 1, GL_FALSE, mesh_object.local_to_world.elements);

      GLint object_id_location = glGetUniformLocation(opengl_shader.program, "u_object_id");
      glProgramUniform1ui(opengl_shader.program, object_id_location, mesh_object.id);

      UseMesh(opengl_mesh);
      DrawSubMesh(opengl_mesh.sub_meshes[mesh_object.sub_mesh_index]);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::DrawRenderBounds(const BoundingBox &bounds) {
    Color color = Color::Red();

    Array<OpenGLImmediateVertex> data;
    data.Resize(8);

    data[0].position = Vector3(bounds.min.x, bounds.min.y, bounds.min.z);
    data[0].color = color;
    data[1].position = Vector3(bounds.min.x, bounds.min.y, bounds.max.z);
    data[1].color = color;
    data[2].position = Vector3(bounds.min.x, bounds.max.y, bounds.min.z);
    data[2].color = color;
    data[3].position = Vector3(bounds.min.x, bounds.max.y, bounds.max.z);
    data[3].color = color;
    data[4].position = Vector3(bounds.max.x, bounds.min.y, bounds.min.z);
    data[4].color = color;
    data[5].position = Vector3(bounds.max.x, bounds.min.y, bounds.max.z);
    data[5].color = color;
    data[6].position = Vector3(bounds.max.x, bounds.max.y, bounds.min.z);
    data[6].color = color;
    data[7].position = Vector3(bounds.max.x, bounds.max.y, bounds.max.z);
    data[7].color = color;

    glNamedBufferSubData(m_storage.GetStatic().render_bounds_vertex_buffer, 0, data.GetLength() * sizeof(OpenGLImmediateVertex), data.GetData());

    glBindVertexArray(m_storage.GetStatic().render_bounds_vertex_array);
    glDrawElementsBaseVertex(GL_LINES, 24, GL_UNSIGNED_INT, 0, 0);
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::UseRenderTexture(RenderTargetId render_target_id) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.UseRenderTexture");
    OpenGLDebugGroup debug_group("UseRenderTexture");

    GLuint framebuffer = 0;
    if (render_target_id.id != RenderTargetId::Default().id) {
      const OpenGLRenderTexture &opengl_render_texture = m_storage.GetRenderTexture(render_target_id.id);

      // We have to specify that we want to draw into all color attachments of the render texture.          
      uint32 color_attachment_count = opengl_render_texture.color_attachment_count;
      Array<GLenum> buffers(color_attachment_count);
      for (GLenum i = 0; i < color_attachment_count; i++) {
        buffers[i] = GL_COLOR_ATTACHMENT0 + i;
      }
      glNamedFramebufferDrawBuffers(opengl_render_texture.framebuffer, color_attachment_count, buffers.GetData());

      framebuffer = opengl_render_texture.framebuffer;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::UseShader(const OpenGLShader &opengl_shader) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.UseShader");
    OpenGLDebugGroup debug_group("UseShader");

    ShaderAttributes attributes = opengl_shader.attributes;
    switch (attributes.z_write) {
      case ShaderZWrite::On:
      {
        glDepthMask(GL_TRUE);
        break;
      }
      case ShaderZWrite::Off:
      {
        glDepthMask(GL_FALSE);
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }
    switch (attributes.blending_mode) {
      case ShaderBlendingMode::On:
      {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
      }
      case ShaderBlendingMode::Off:
      {
        glDisable(GL_BLEND);
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }
    switch (attributes.culling_mode) {
      case ShaderCullingMode::Off:
      {
        glDisable(GL_CULL_FACE);
        break;
      }
      case ShaderCullingMode::Front:
      {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        break;
      }
      case ShaderCullingMode::Back:
      {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }

    glUseProgram(opengl_shader.program);
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::UseMaterial(const OpenGLShader &opengl_shader, const OpenGLMaterial &opengl_material) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.UseMaterial");
    OpenGLDebugGroup debug_group("UseMaterial");

    uint32 texture_unit = 0;
    MaterialPropertyIndex index = 0;
    for (const MaterialProperty &property : opengl_material.properties) {
      GLint location = -1;
      if (index < opengl_shader.locations.GetLength()) {
        location = opengl_shader.locations[index++];
      }

      switch (property.type) {
        case ShaderPropertyType::Int:
        {
          glProgramUniform1i(opengl_shader.program, location, property.storage.int32);
          break;
        }
        case ShaderPropertyType::Float:
        {
          glProgramUniform1f(opengl_shader.program, location, property.storage.float32);
          break;
        }
        case ShaderPropertyType::Vector:
        {
          Vector4 vector = property.storage.vector4;
          glProgramUniform4f(opengl_shader.program, location, vector.x, vector.y, vector.z, vector.w);
          break;
        }
        case ShaderPropertyType::Color:
        {
          Color color = property.storage.color;
          glProgramUniform4f(opengl_shader.program, location, color.r, color.g, color.b, color.a);
          break;
        }
        case ShaderPropertyType::Matrix:
        {
          glProgramUniformMatrix4fv(opengl_shader.program, location, 1, GL_FALSE, property.storage.matrix4x4.elements);
          break;
        }
        case ShaderPropertyType::Texture:
        {
          SetMaterialTextureProperty(property.storage.texture, texture_unit, opengl_shader.program, location);
          texture_unit++;
          break;
        }
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
      }
    }
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::SetMaterialTextureProperty(ShaderPropertyStorage::Texture texture_property, uint32 texture_unit, GLuint program, GLuint location) {
    // NOTE: This setting of textures is quite expensive and could probably be done more performant.

    GLuint texture = 0;
    if (texture_property.dimension == TextureDimension::RenderTexture) {
      // TODO: We should do more validation when setting a render texture.
      auto it = m_storage.FindRenderTexture(texture_property.id);
      if (it == m_storage.GetRenderTextureEnd()) {
        HYP_LOG_ERROR("OpenGL", "Trying to set non existing render texture as shader property!");
        return;
      } else {
        const OpenGLRenderTextureAttachment &opengl_attachment = it->second.attachments[texture_property.render_texture_attchment_index];
        texture = opengl_attachment.attachment;
      }
    } else {
      auto it = m_storage.FindTexture2D(texture_property.id);
      if (it == m_storage.GetTexture2DEnd()) {
        HYP_LOG_ERROR("OpenGL", "Trying to set non existing texture as shader property!");
        return;
      } else {
        texture = it->second.texture;
      }
    }

    glBindTextureUnit(texture_unit, texture);
    glProgramUniform1i(program, location, texture_unit);
  }

  //--------------------------------------------------------------
  void OpenGLRenderDriver::UseMesh(const OpenGLMesh &opengl_mesh) {
    OpenGLDebugGroup debug_group("UseMesh");

    glBindVertexArray(opengl_mesh.vertex_array);
  }

}