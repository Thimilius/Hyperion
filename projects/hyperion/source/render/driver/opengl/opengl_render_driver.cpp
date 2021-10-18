//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/opengl/opengl_render_driver.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/render/driver/opengl/opengl_render_driver_shader_compiler.hpp"
#include "hyperion/render/driver/opengl/opengl_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void OpenGLRenderDriver::Initialize() {
        {
            glEnable(GL_DEPTH_TEST);
            glFrontFace(GL_CW);
        }

        {
            const char *fullscreen_vertex = R"(
                #version 450 core

                out V2F {
	                vec2 texture0;
                } o_v2f;

                void main() {
	                vec2 vertices[3] = vec2[3](vec2(-1.0, -1.0f), vec2(-1.0, 3.0), vec2(3.0f, -1.0));
	                vec4 position = vec4(vertices[gl_VertexID], 0.0, 1.0);

	                o_v2f.texture0 = 0.5 * position.xy + vec2(0.5);

	                gl_Position = position;
                }
            )";
            const char *fullscreen_fragment = R"(
                #version 450 core

                layout(location = 0) out vec4 o_color;

                in V2F {
	                vec2 texture0;
                } i_v2f;

                uniform sampler2D u_texture;

                void main() {
	                o_color = texture(u_texture, i_v2f.texture0);
                }
            )";
            m_state.fullscreen_shader = OpenGLRenderDriverShaderCompiler::Compile(fullscreen_vertex, fullscreen_fragment).program;
            glCreateVertexArrays(1, &m_state.fullscreen_vertex_array);
        }

        {
            glCreateBuffers(1, &m_state.camera_uniform_buffer);
            glNamedBufferData(m_state.camera_uniform_buffer, sizeof(OpenGLUniformBufferCamera), nullptr, GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_state.camera_uniform_buffer);
        }

        {
            glCreateBuffers(1, &m_state.render_bounds_vertex_buffer);
            glNamedBufferData(m_state.render_bounds_vertex_buffer, 8 * sizeof(OpenGLImmediateVertex), nullptr, GL_DYNAMIC_DRAW);

            Array<uint32> indices = {
                0, 1,
                1, 5,
                5, 4,
                4, 0,

                0, 2,
                1, 3,
                5, 7,
                4, 6,

                2, 3,
                3, 7,
                7, 6,
                6, 2,
            };

            glCreateBuffers(1, &m_state.render_bounds_index_buffer);
            glNamedBufferData(m_state.render_bounds_index_buffer, indices.GetLength() * sizeof(uint32), indices.GetData(), GL_STATIC_DRAW);

            glCreateVertexArrays(1, &m_state.render_bounds_vertex_array);
            glVertexArrayVertexBuffer(m_state.render_bounds_vertex_array, 0, m_state.render_bounds_vertex_buffer, 0, sizeof(OpenGLImmediateVertex));
            glVertexArrayElementBuffer(m_state.render_bounds_vertex_array, m_state.render_bounds_index_buffer);

            glEnableVertexArrayAttrib(m_state.render_bounds_vertex_array, 0);
            glVertexArrayAttribFormat(m_state.render_bounds_vertex_array, 0, 3, GL_FLOAT, false, 0);
            glVertexArrayAttribBinding(m_state.render_bounds_vertex_array, 0, 0);
            glEnableVertexArrayAttrib(m_state.render_bounds_vertex_array, 3);
            glVertexArrayAttribFormat(m_state.render_bounds_vertex_array, 3, 4, GL_FLOAT, false, sizeof(Vector3));
            glVertexArrayAttribBinding(m_state.render_bounds_vertex_array, 3, 0);
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::Render(RenderFrame *render_frame) {
        UnloadAssets(render_frame->GetContext());
        LoadAssets(render_frame->GetContext());

        ExecuteRenderFrameCommands(render_frame);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::Shutdown() {
        // NOTE: We could bother unloading every asset on the GPU but that is pretty much just a waste of time.
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::ExecuteRenderFrameCommands(RenderFrame *render_frame) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.ExecuteRenderFrameCommands");

        const RenderFrameContext &render_frame_context = render_frame->GetContext();

        const Array<RenderFrameCommand> &frame_commands = render_frame->GetCommands();
        for (const RenderFrameCommand &frame_command : frame_commands) {
            switch (frame_command.type) {
                case RenderFrameCommandType::SetCamera: {
                    HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.SetCamera");

                    const RenderFrameCommandSetCamera &set_camera = std::get<RenderFrameCommandSetCamera>(frame_command.data);
                    m_state.camera_index = set_camera.camera_index;

                    const RenderFrameContextCamera &render_frame_context_camera = render_frame_context.GetCameras()[m_state.camera_index];

                    OpenGLUniformBufferCamera uniform_buffer_camera;
                    uniform_buffer_camera.camera_view_matrix = render_frame_context_camera.view_matrix;
                    uniform_buffer_camera.camera_projection_matrix = render_frame_context_camera.projection_matrix;
                    glNamedBufferSubData(m_state.camera_uniform_buffer, 0, sizeof(OpenGLUniformBufferCamera), &uniform_buffer_camera);

                    break;
                }
                case RenderFrameCommandType::ExecuteCommandBuffer: {
                    HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.ExecuteCommandBuffer");

                    const RenderFrameCommandExecuteCommandBuffer &execute_command_buffer = std::get<RenderFrameCommandExecuteCommandBuffer>(frame_command.data);

                    const Array<RenderFrameCommandBufferCommand> &buffer_commands = execute_command_buffer.command_buffer.GetCommands();
                    for (const RenderFrameCommandBufferCommand &buffer_command : buffer_commands) {
                        switch (buffer_command.type) {
                            case RenderFrameCommandBufferCommandType::ClearRenderTarget: {
                                HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommandBufferCommand.Clear");

                                const RenderFrameCommandBufferCommandClearRenderTarget &clear_render_target = std::get<RenderFrameCommandBufferCommandClearRenderTarget>(buffer_command.data);

                                const RenderFrameContextCamera &render_frame_context_camera = render_frame_context.GetCameras()[m_state.camera_index];
                                const CameraViewport &viewport = render_frame_context_camera.viewport;
                                glViewport(viewport.x, viewport.y, viewport.width, viewport.height);

                                Color background_color = clear_render_target.color;
                                glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
                                
                                // We have to make sure that we can clear the depth buffer by enabling the depth mask.
                                glDepthMask(GL_TRUE);
                                glClear(OpenGLUtilities::GetClearFlags(clear_render_target.flags));
                                break;
                            }
                            case RenderFrameCommandBufferCommandType::SetRenderTarget: {
                                HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommandBufferCommand.SetRenderTarget");

                                const RenderFrameCommandBufferCommandSetRenderTarget &set_render_target = std::get<RenderFrameCommandBufferCommandSetRenderTarget>(buffer_command.data);

                                GLuint framebuffer = 0;
                                if (set_render_target.id.id != RenderTargetId::Default().id) {
                                    OpenGLRenderTexture &opengl_render_texture = m_opengl_render_textures.Get(set_render_target.id.id);

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
                                break;
                            }
                            case RenderFrameCommandBufferCommandType::Blit: {
                                HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommandBufferCommand.Blit");

                                const RenderFrameCommandBufferCommandBlit &blit = std::get<RenderFrameCommandBufferCommandBlit>(buffer_command.data);
                                
                                GLint source_width = Display::GetWidth();
                                GLint source_height = Display::GetHeight();
                                GLuint source_framebuffer = 0;
                                GLuint source_color_attachment = 0;
                                if (blit.source.id != RenderTargetId::Default().id) {
                                    const OpenGLRenderTexture &opengl_render_texture = m_opengl_render_textures.Get(blit.source.id);
                                    source_width = opengl_render_texture.width;
                                    source_height = opengl_render_texture.height;
                                    source_framebuffer = opengl_render_texture.framebuffer;

                                    source_color_attachment = opengl_render_texture.attachments.Get(1).attachment;
                                }

                                GLint destination_width = Display::GetWidth();
                                GLint destination_height = Display::GetHeight();
                                GLuint destination_framebuffer = 0;
                                if (blit.destination.id != RenderTargetId::Default().id) {
                                    const OpenGLRenderTexture &opengl_render_texture = m_opengl_render_textures.Get(blit.destination.id);
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

                                    glUseProgram(m_state.fullscreen_shader);
                                    glBindTextureUnit(0, source_color_attachment);
                                    glBindVertexArray(m_state.fullscreen_vertex_array);
                                    glDrawArrays(GL_TRIANGLES, 0, 3);

                                    glBindFramebuffer(GL_FRAMEBUFFER, current_framebuffer);
                                }

                                break;
                            }
                            case RenderFrameCommandBufferCommandType::SetGlobalBuffer: {
                                HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommandBufferCommand.SetGlobalBuffer");

                                const RenderFrameCommandBufferCommandSetGlobalBuffer &set_global_buffer = std::get<RenderFrameCommandBufferCommandSetGlobalBuffer>(buffer_command.data);
                                auto &data = set_global_buffer.render_buffer.GetData();

                                // HACK: This is kind of stupid.
                                // We need something more sophisticated which abstracts the proper shader uniform names and binding points.
                                GLuint buffer_id = -1;
                                if (set_global_buffer.id == 0) {
                                    buffer_id = m_state.lighting_uniform_buffer;

                                    if (buffer_id == -1) {
                                        glCreateBuffers(1, &buffer_id);
                                        glNamedBufferData(buffer_id, data.GetLength(), data.GetData(), GL_DYNAMIC_DRAW);
                                        glBindBufferBase(GL_UNIFORM_BUFFER, 1, buffer_id);

                                        m_state.lighting_uniform_buffer = buffer_id;

                                        return;
                                    }
                                }

                                glNamedBufferSubData(buffer_id, 0, data.GetLength(), data.GetData());
                                break;
                            }
                            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
                        }
                    }
                    
                    break;
                }
                case RenderFrameCommandType::DrawMeshes: {
                    HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.DrawMeshes");

                    const RenderFrameCommandDrawMeshes &draw_meshes = std::get<RenderFrameCommandDrawMeshes>(frame_command.data);

                    PrepareObjects(render_frame, draw_meshes.sorted_objects, draw_meshes.drawing_parameters);
                    DrawMeshes(render_frame_context.GetEnvironment(), render_frame_context.GetLights(), draw_meshes.drawing_parameters);

                    break;
                }
                case RenderFrameCommandType::DrawUI: {
                    HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.DrawUI");

                    const RenderFrameCommandDrawUI &draw_ui = std::get<RenderFrameCommandDrawUI>(frame_command.data);

                    DrawUI(render_frame_context.GetUIObjects());

                    break;
                }
                case RenderFrameCommandType::DrawGizmos: {
                    HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.DrawGizmos");

                    const RenderFrameCommandDrawGizmos &draw_gizmos = std::get<RenderFrameCommandDrawGizmos>(frame_command.data);

                    glDepthMask(GL_FALSE);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                    const OpenGLShader &opengl_shader = m_opengl_shaders.Get(draw_gizmos.shader_id);
                    glUseProgram(opengl_shader.program);

                    GLint model_location = glGetUniformLocation(opengl_shader.program, "u_model");

                    if (draw_gizmos.grid.should_draw) {
                        glProgramUniformMatrix4fv(opengl_shader.program, model_location, 1, GL_FALSE, draw_gizmos.grid.local_to_world.elements);

                        const OpenGLMesh &opengl_mesh = m_opengl_meshes.Get(draw_gizmos.grid.mesh_id);
                        glBindVertexArray(opengl_mesh.vertex_array);

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

            auto opengl_shader_it = m_opengl_shaders.Find(shader_id);
            if (opengl_shader_it == m_opengl_shaders.end()) {
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
                new_grouped_shader.shader = &m_opengl_shaders.Get(shader_id);
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
                new_grouped_material.material = &m_opengl_materials.Get(material_id);
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
                new_grouped_material.mesh = &m_opengl_meshes.Get(mesh_id);
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
            float32 width = static_cast<float32>(Display::GetWidth());
            float32 height = static_cast<float32>(Display::GetHeight());

            OpenGLUniformBufferCamera uniform_buffer_camera;
            uniform_buffer_camera.camera_view_matrix = Matrix4x4::Identity();
            uniform_buffer_camera.camera_projection_matrix = Matrix4x4::Orthographic(0.0f, width, 0.0f, height, -1.0f, 0.0f);
            glNamedBufferSubData(m_state.camera_uniform_buffer, 0, sizeof(OpenGLUniformBufferCamera), &uniform_buffer_camera);
        }

        for (const RenderFrameContextObjectUI &element : elements) {
            const OpenGLShader &opengl_shader = m_opengl_shaders.Get(element.shader_id);
            UseShader(opengl_shader);

            const OpenGLMaterial &opengl_material = m_opengl_materials.Get(element.material_id);
            UseMaterial(opengl_shader, opengl_material);
            {
                GLint color_location = glGetUniformLocation(opengl_shader.program, "u_color");
                if (color_location >= 0) {
                    Color color = element.color;
                    glProgramUniform4f(opengl_shader.program, color_location, color.r, color.g, color.b, color.a);
                }

                GLint texture_location = glGetUniformLocation(opengl_shader.program, "u_texture");
                if (texture_location >= 0) {
                    const OpenGLTexture &opengl_texture = m_opengl_textures.Get(element.texture_id);

                    glBindTextureUnit(0, opengl_texture.texture);
                    glProgramUniform1i(opengl_shader.program, texture_location, 0);
                }
            }

            OpenGLMesh &opengl_mesh = m_opengl_meshes.Get(element.mesh_id);
            UseMesh(opengl_mesh);

            DrawSubMesh(opengl_mesh.sub_meshes[0]);
        }
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

        glNamedBufferSubData(m_state.render_bounds_vertex_buffer, 0, data.GetLength() * sizeof(OpenGLImmediateVertex), data.GetData());

        glBindVertexArray(m_state.render_bounds_vertex_array);
        glDrawElementsBaseVertex(GL_LINES, 24, GL_UNSIGNED_INT, 0, 0);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::UseShader(const OpenGLShader &opengl_shader) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.UseShader");

        ShaderAttributes attributes = opengl_shader.attributes;
        switch (attributes.z_write) {
            case ShaderZWrite::On: {
                glDepthMask(GL_TRUE);
                break;
            }
            case ShaderZWrite::Off: {
                glDepthMask(GL_FALSE);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
        switch (attributes.blending_mode) {
            case ShaderBlendingMode::On: {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;
            }
            case ShaderBlendingMode::Off: {
                glDisable(GL_BLEND);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
        switch (attributes.culling_mode) {
            case ShaderCullingMode::Off: {
                glDisable(GL_CULL_FACE);
                break;
            }
            case ShaderCullingMode::Front: {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
                break;
            }
            case ShaderCullingMode::Back: {
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

        uint32 texture_unit = 0;
        MaterialPropertyIndex index = 0;
        for (const MaterialProperty &property : opengl_material.properties) {
            GLint location = opengl_shader.locations[index++];

            switch (property.type) {
                case ShaderPropertyType::Int: {
                    glProgramUniform1i(opengl_shader.program, location, property.storage.int32);
                    break;
                }
                case ShaderPropertyType::Float: {
                    glProgramUniform1f(opengl_shader.program, location, property.storage.float32);
                    break;
                }
                case ShaderPropertyType::Vector: {
                    Vector4 vector = property.storage.vector4;
                    glProgramUniform4f(opengl_shader.program, location, vector.x, vector.y, vector.z, vector.w);
                    break;
                }
                case ShaderPropertyType::Color: {
                    Color color = property.storage.color;
                    glProgramUniform4f(opengl_shader.program, location, color.r, color.g, color.b, color.a);
                    break;
                }
                case ShaderPropertyType::Matrix: {
                    glProgramUniformMatrix4fv(opengl_shader.program, location, 1, GL_FALSE, property.storage.matrix4x4.elements);
                    break;
                }
                case ShaderPropertyType::Texture: {
                    // TODO: This updating of textures is quite expensive and should be done more performant.
                    auto it = m_opengl_textures.Find(property.storage.texture);
                    if (it == m_opengl_textures.end()) {
                        HYP_LOG_ERROR("OpenGL", "Trying to set non existing texture shader property!");
                    } else {
                        GLuint texture = it->second.texture;
                        glBindTextureUnit(texture_unit, texture);
                        glProgramUniform1i(opengl_shader.program, location, texture_unit);
                        texture_unit++;
                    }
                    break;
                }
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
            }
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::UseMesh(const OpenGLMesh &opengl_mesh) {
        glBindVertexArray(opengl_mesh.vertex_array);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::UnloadAssets(RenderFrameContext &render_frame_context) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.UnloadAssets");

        for (AssetId shader_id : render_frame_context.GetShaderAssetsToUnload()) {
            glDeleteProgram(m_opengl_shaders.Get(shader_id).program);
            m_opengl_shaders.Remove(shader_id);
        }
        for (AssetId texture_2d_id : render_frame_context.GetTexture2DAssetsToUnload()) {
            GLuint texture = m_opengl_textures.Get(texture_2d_id).texture;
            glDeleteTextures(1, &texture);
            m_opengl_textures.Remove(texture_2d_id);
        }
        for (AssetId render_texture_id : render_frame_context.GetRenderTextureAssetsToUnload()) {
            OpenGLRenderTexture &opengl_render_texture = m_opengl_render_textures.Get(render_texture_id);
            for (OpenGLRenderTextureAttachment &opengl_attachment : opengl_render_texture.attachments) {
                if (opengl_attachment.format == RenderTextureFormat::Depth24Stencil8) {
                    glDeleteRenderbuffers(1, &opengl_attachment.attachment);
                } else {
                    glDeleteTextures(1, &opengl_attachment.attachment);
                }
            }
            m_opengl_render_textures.Remove(render_texture_id);
        }
        for (AssetId material_id : render_frame_context.GetMaterialAssetsToUnload()) {
            m_opengl_materials.Remove(material_id);
        }
        for (AssetId mesh_id : render_frame_context.GetMeshAssetsToUnload()) {
            OpenGLMesh &opengl_mesh = m_opengl_meshes.Get(mesh_id);

            glDeleteBuffers(1, &opengl_mesh.vertex_buffer);
            glDeleteBuffers(1, &opengl_mesh.index_buffer);
            glDeleteVertexArrays(1, &opengl_mesh.vertex_array);

            m_opengl_meshes.Remove(mesh_id);
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::LoadAssets(RenderFrameContext &render_frame_context) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.LoadAssets");

        // The order in which we load the assets is important.
        // For example a material might reference a shader or texture which should always be loaded first.

        for (RenderFrameContextAssetShader &shader : render_frame_context.GetShaderAssetsToLoad()) {
            LoadShader(shader);
        }
        for (RenderFrameContextAssetTexture2D &texture_2d : render_frame_context.GetTexture2DAssetsToLoad()) {
            LoadTexture2D(texture_2d);
        }
        for (RenderFrameContextAssetRenderTexture &render_texture : render_frame_context.GetRenderTextureAssetsToLoad()) {
            LoadRenderTexture(render_texture);
        }
        for (RenderFrameContextAssetMaterial &material : render_frame_context.GetMaterialAssetsToLoad()) {
            LoadMaterial(material);
        }
        for (RenderFrameContextAssetMesh &mesh : render_frame_context.GetMeshAssetsToLoad()) {
            LoadMesh(mesh);
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::LoadTexture2D(RenderFrameContextAssetTexture2D &texture_2d) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.LoadTexture2D");

        OpenGLTexture opengl_texture;
        opengl_texture.id = texture_2d.id;

        glCreateTextures(GL_TEXTURE_2D, 1, &opengl_texture.texture);

        SetTextureAttributes(opengl_texture.texture, texture_2d.parameters.attributes);

        TextureFormat format = texture_2d.parameters.format;
        GLsizei width = texture_2d.parameters.width;
        GLsizei height = texture_2d.parameters.height;
        GLenum internal_format = OpenGLUtilities::GetTextureInternalFormat(format);
        GLsizei mipmap_count = Math::Max(texture_2d.mipmap_count, 1);
        glTextureStorage2D(opengl_texture.texture, mipmap_count, internal_format, width, height);

        OpenGLUtilities::FlipTextureHorizontally(texture_2d.parameters.width, texture_2d.parameters.height, format, texture_2d.pixels);
        GLenum format_value = OpenGLUtilities::GetTextureFormat(format);
        GLenum format_type = OpenGLUtilities::GetTextureFormatType(format);
        OpenGLUtilities::SetUnpackAlignmentForTextureFormat(format);
        glTextureSubImage2D(opengl_texture.texture, 0, 0, 0, width, height, format_value, format_type, texture_2d.pixels.GetData());

        if (texture_2d.parameters.attributes.use_mipmaps) {
            glGenerateTextureMipmap(opengl_texture.texture);
        }

        m_opengl_textures.Insert(texture_2d.id, opengl_texture);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::LoadRenderTexture(RenderFrameContextAssetRenderTexture &render_texture) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.LoadRenderTexture");

        // TODO: We should do some sort of validation.

        uint64 attachment_count = render_texture.parameters.attachments.GetLength();
        uint32 width = render_texture.parameters.width;
        uint32 height = render_texture.parameters.height;

        OpenGLRenderTexture opengl_render_texture;
        opengl_render_texture.id = render_texture.id;
        opengl_render_texture.attachments.Resize(attachment_count);
        opengl_render_texture.width = width;
        opengl_render_texture.height = height;

        glCreateFramebuffers(1, &opengl_render_texture.framebuffer);
        
        uint32 color_attachment_index = 0;
        for (uint32 i = 0; i < attachment_count; i++) {
            const RenderTextureAttachment &attachment = render_texture.parameters.attachments[i];
            OpenGLRenderTextureAttachment &opengl_attachment = opengl_render_texture.attachments[i];
            opengl_attachment.format = attachment.format;

            if (attachment.format == RenderTextureFormat::Depth24Stencil8) {
                glCreateRenderbuffers(1, &opengl_attachment.attachment);
                glNamedRenderbufferStorage(opengl_attachment.attachment, GL_DEPTH24_STENCIL8, width, height);
                glNamedFramebufferRenderbuffer(opengl_render_texture.framebuffer, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, opengl_attachment.attachment);
            } else {
                glCreateTextures(GL_TEXTURE_2D, 1, &opengl_attachment.attachment);

                TextureAttributes attributes = attachment.attributes;
                SetTextureAttributes(opengl_attachment.attachment, attributes);

                GLenum internal_format = OpenGLUtilities::GetRenderTextureInternalFormat(attachment.format);
                GLsizei mipmap_count = attributes.use_mipmaps ? Math::Max(render_texture.mipmap_count, 1) : 1;
                glTextureStorage2D(opengl_attachment.attachment, mipmap_count, internal_format, width, height);

                GLenum attachment_index = GL_COLOR_ATTACHMENT0 + color_attachment_index;
                glNamedFramebufferTexture(opengl_render_texture.framebuffer, attachment_index, opengl_attachment.attachment, 0);
                color_attachment_index++;
            }
        }

        opengl_render_texture.color_attachment_count = color_attachment_index;

        if (glCheckNamedFramebufferStatus(opengl_render_texture.framebuffer, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
            m_opengl_render_textures.Insert(render_texture.id, opengl_render_texture);
        } else {
            HYP_LOG_ERROR("OpenGL", "Failed to create render texture!");
        }

    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::LoadShader(RenderFrameContextAssetShader &shader) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.LoadShader");

        // FIXME: Handle shader compilation errors.
        OpenGLShader opengl_shader;
        opengl_shader.id = shader.id;
        opengl_shader.attributes = shader.data.attributes;
        opengl_shader.program = OpenGLRenderDriverShaderCompiler::Compile(shader.data.vertex_source.c_str(), shader.data.fragment_source.c_str()).program;


        opengl_shader.locations.Reserve(shader.data.properties.GetLength());
        for (const ShaderProperty &property : shader.data.properties) {
            opengl_shader.locations.Add(glGetUniformLocation(opengl_shader.program, property.name.c_str()));
        }

        m_opengl_shaders.Insert(shader.id, opengl_shader);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::LoadMaterial(RenderFrameContextAssetMaterial &material) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.LoadMaterial");

        auto material_it = m_opengl_materials.Find(material.id);
        if (material_it == m_opengl_materials.end()) {
            OpenGLMaterial opengl_material;
            opengl_material.id = material.id;
            opengl_material.shader = &m_opengl_shaders.Get(material.shader_id);;
            opengl_material.properties = std::move(material.properties);

            m_opengl_materials.Insert(material.id, opengl_material);
        } else {
            OpenGLMaterial &opengl_material = material_it->second;
            opengl_material.properties = std::move(material.properties);
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::LoadMesh(RenderFrameContextAssetMesh &mesh) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.LoadMesh");

        const MeshData &data = mesh.data;
        const MeshVertexFormat &vertex_format = mesh.vertex_format;

        bool8 has_normals = data.normals.GetLength() > 0;
        bool8 has_colors = data.colors.GetLength() > 0;
        bool8 has_texture0 = data.texture0.GetLength() > 0;

        uint32 vertex_count = static_cast<uint32>(data.positions.GetLength());
        Array<byte> vertices(vertex_count * vertex_format.stride);
        for (uint32 i = 0; i < vertex_count; i++) {
            uint32 index = i * vertex_format.stride;

            Vector3 &position = reinterpret_cast<Vector3 &>(vertices[index]);
            position = data.positions[i];
            index += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_POSITION;

            if (has_normals) {
                Vector3 &normal = reinterpret_cast<Vector3 &>(vertices[index]);
                normal = data.normals[i];
                index += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_NORMAL;
            }
            if (has_colors) {
                Vector4 &color = reinterpret_cast<Vector4 &>(vertices[index]);
                color = data.colors[i];
                index += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_COLOR;
            }
            if (has_texture0) {
                Vector2 &texture0 = reinterpret_cast<Vector2 &>(vertices[index]);
                texture0 = data.texture0[i];
                index += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_TEXTURE0;
            }
        }

        OpenGLMesh opengl_mesh;
        opengl_mesh.id = mesh.id;
        opengl_mesh.sub_meshes = std::move(mesh.sub_meshes);

        glCreateBuffers(1, &opengl_mesh.vertex_buffer);
        glNamedBufferData(opengl_mesh.vertex_buffer, vertices.GetLength(), vertices.GetData(), GL_STATIC_DRAW);

        glCreateBuffers(1, &opengl_mesh.index_buffer);
        glNamedBufferData(opengl_mesh.index_buffer, data.indices.GetLength() * sizeof(data.indices[0]), data.indices.GetData(), GL_STATIC_DRAW);
        opengl_mesh.index_count = static_cast<GLsizei>(data.indices.GetLength());

        GLsizei stride = vertex_format.stride;
        GLuint relative_offset = 0;
        glCreateVertexArrays(1, &opengl_mesh.vertex_array);
        glVertexArrayVertexBuffer(opengl_mesh.vertex_array, 0, opengl_mesh.vertex_buffer, 0, stride);
        glVertexArrayElementBuffer(opengl_mesh.vertex_array, opengl_mesh.index_buffer);

        for (VertexAttribute vertex_attribute : vertex_format.attributes) {
            GLuint attribute_index = OpenGLUtilities::GetAttributeIndexForVertextAttributeSize(vertex_attribute.kind);
            GLuint binding_index = 0;
            GLint size = vertex_attribute.dimension;
            GLenum type = OpenGLUtilities::GetVertexAttributeType(vertex_attribute.type);

            glEnableVertexArrayAttrib(opengl_mesh.vertex_array, attribute_index);
            glVertexArrayAttribFormat(opengl_mesh.vertex_array, attribute_index, size, type, false, relative_offset);
            glVertexArrayAttribBinding(opengl_mesh.vertex_array, attribute_index, binding_index);

            relative_offset += OpenGLUtilities::GetVertexAttributeSizeForVertexAttribute(vertex_attribute.type, vertex_attribute.dimension);
        }

        m_opengl_meshes.Insert(mesh.id, opengl_mesh);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::SetTextureAttributes(GLuint texture, TextureAttributes attributes) {
        GLint wrap_mode = OpenGLUtilities::GetTextureWrapMode(attributes.wrap_mode);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, wrap_mode);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, wrap_mode);

        GLint min_filter = OpenGLUtilities::GetTextureMinFilter(attributes.filter);
        GLint mag_filter = OpenGLUtilities::GetTextureMaxFilter(attributes.filter);
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, mag_filter);

        GLfloat anisotropic_filter_value = OpenGLUtilities::GetTextureAnisotropicFilter(attributes.anisotropic_filter);
        glTextureParameterf(texture, GL_TEXTURE_MAX_ANISOTROPY, anisotropic_filter_value);
    }

}