//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

#include <array>

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/opengl/opengl_render_driver.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/render/driver/opengl/opengl_render_driver_shader_compiler.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    uint32 GetBytesPerPixelForTextureFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGBA32: return 4;
            case TextureFormat::RGB24: return 3;
            case TextureFormat::R8: return 1;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    //--------------------------------------------------------------
    void SetUnpackAlignmentForTextureFormat(TextureFormat format) {
        GLint alignment = 4;
        switch (format) {
            case TextureFormat::RGB24: alignment = 4; break;
            case TextureFormat::RGBA32: alignment = 4; break;
            case TextureFormat::R8: alignment = 1; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    }

    //--------------------------------------------------------------
    void FlipTextureHorizontally(uint32 width, uint32 height, TextureFormat format, Array<byte> &pixels) {
        uint32 stride = width * GetBytesPerPixelForTextureFormat(format);
        Array<byte> temp_buffer(stride);
        byte *temp_buffer_data = temp_buffer.GetData();

        byte *data = pixels.GetData();

        for (uint32 row = 0; row < height / 2; row++) {
            byte *source = data + (row * stride);
            byte *destination = data + (((height - 1) - row) * stride);

            std::memcpy(temp_buffer_data, source, stride);
            std::memcpy(source, destination, stride);
            std::memcpy(destination, temp_buffer_data, stride);
        }
    }

    //--------------------------------------------------------------
    GLenum GetTextureFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGB24: return GL_RGB;
            case TextureFormat::RGBA32: return GL_RGBA;
            case TextureFormat::R8: return GL_RED;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    //--------------------------------------------------------------
    GLenum GetTextureFormatType(TextureFormat format_type) {
        switch (format_type) {
            case TextureFormat::RGB24: return GL_UNSIGNED_BYTE;
            case TextureFormat::RGBA32: return GL_UNSIGNED_BYTE;
            case TextureFormat::R8: return GL_UNSIGNED_BYTE;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    //--------------------------------------------------------------
    GLenum GetTextureInternalFormat(TextureFormat internal_format) {
        switch (internal_format) {
            case TextureFormat::RGB24: return GL_RGB8;
            case TextureFormat::RGBA32: return GL_RGBA8;
            case TextureFormat::R8: return GL_R8;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    //--------------------------------------------------------------
    GLint GetTextureWrapMode(TextureWrapMode wrap_mode) {
        switch (wrap_mode) {
            case TextureWrapMode::Clamp: return GL_CLAMP_TO_EDGE;
            case TextureWrapMode::Border: return GL_CLAMP_TO_BORDER;
            case TextureWrapMode::Repeat: return GL_REPEAT;
            case TextureWrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    //--------------------------------------------------------------
    GLint GetTextureMinFilter(TextureFilter filter) {
        switch (filter) {
            case TextureFilter::Point: return GL_NEAREST_MIPMAP_NEAREST;
            case TextureFilter::Bilinear: return GL_LINEAR_MIPMAP_NEAREST;
            case TextureFilter::Trilinear: return GL_LINEAR_MIPMAP_LINEAR;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return GL_NEAREST_MIPMAP_NEAREST;
        }
    }

    //--------------------------------------------------------------
    GLint GetTextureMaxFilter(TextureFilter filter) {
        switch (filter) {
            case TextureFilter::Point: return GL_NEAREST;
            case TextureFilter::Bilinear: return GL_LINEAR;
            case TextureFilter::Trilinear: return GL_LINEAR;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return GL_NEAREST;
        }
    }

    //--------------------------------------------------------------
    GLfloat GetTextureAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) {
        switch (anisotropic_filter) {
            case TextureAnisotropicFilter::None: return 1.0f;
            case TextureAnisotropicFilter::Times2: return 2.0f;
            case TextureAnisotropicFilter::Times4: return 4.0f;
            case TextureAnisotropicFilter::Times8: return 8.0f;
            case TextureAnisotropicFilter::Times16: return 16.0f;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 1.0f;
        }
    }

    //--------------------------------------------------------------
    GLenum GetGLTopology(MeshTopology mesh_topology) {
        switch (mesh_topology) {
            case MeshTopology::Points: return GL_POINTS;
            case MeshTopology::Lines: return GL_LINES;
            case MeshTopology::LineStrip: return GL_LINE_STRIP;
            case MeshTopology::Triangles: return GL_TRIANGLES;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    //--------------------------------------------------------------
    GLuint GetGLAttributeIndexForVertextAttributeSize(VertexAttributeKind kind) {
        switch (kind) {
            case VertexAttributeKind::Position: return 0;
            case VertexAttributeKind::Normal: return 1;
            case VertexAttributeKind::Tangent: return 2;
            case VertexAttributeKind::Color: return 3;
            case VertexAttributeKind::Texture0: return 4;
            case VertexAttributeKind::Texture1: return 5;
            case VertexAttributeKind::Texture2: return 6;
            case VertexAttributeKind::Texture3: return 7;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    //--------------------------------------------------------------
    GLenum GetGLVertexAttributeType(VertexAttributeType vertex_attribute_type) {
        switch (vertex_attribute_type) {
            case VertexAttributeType::Float32: return GL_FLOAT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    //--------------------------------------------------------------
    GLuint GetGLVertexAttributeSizeForVertexAttribute(VertexAttributeType vertex_attribute_type, uint32 dimension) {
        switch (vertex_attribute_type) {
            case VertexAttributeType::Float32: return sizeof(float32) * dimension;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::Initialize() {
        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CW);

        glCreateBuffers(1, &m_state.camera_uniform_buffer);
        glNamedBufferData(m_state.camera_uniform_buffer, sizeof(OpenGLUniformBufferCamera), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_state.camera_uniform_buffer);
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

                                const RenderFrameContextCamera &render_frame_context_camera = render_frame_context.GetCameras()[m_state.camera_index];

                                const CameraViewport &viewport = render_frame_context_camera.viewport;
                                glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
                                Color background_color = render_frame_context_camera.background_color;
                                glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
                                
                                // We have to make sure that we can clear the depth buffer by enabling the depth mask.
                                glDepthMask(GL_TRUE);
                                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

                    const RenderFrameContextCamera &render_frame_context_camera = render_frame_context.GetCameras()[m_state.camera_index];

                    PrepareObjects(render_frame, draw_meshes.sorted_objects, draw_meshes.drawing_parameters);
                    RenderCamera(render_frame_context.GetEnvironment(), render_frame_context.GetLights(), render_frame_context_camera, draw_meshes.drawing_parameters);

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

            if ((render_frame_context_object_mesh.layer_mask & drawing_parameters.filter_mask) != render_frame_context_object_mesh.layer_mask) {
                continue;
            }

            AssetId material_id = render_frame_context_object_mesh.material->GetAssetInfo().id;
            AssetId shader_id = render_frame_context_object_mesh.material->GetShader()->GetAssetInfo().id;
            AssetId mesh_id = render_frame_context_object_mesh.mesh->GetAssetInfo().id;

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
    void OpenGLRenderDriver::RenderCamera(const RenderFrameContextEnvironment &environment, const Array<RenderFrameContextLight> &lights, const RenderFrameContextCamera &camera, DrawingParametes drawing_parameters) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderCamera");

        for (const GroupedShader &grouped_shader : m_grouped_shaders) {
            HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderGroupedShader");

            const OpenGLShader &opengl_shader = *grouped_shader.shader;
            UseShader(opengl_shader);

            for (const auto [material_id, grouped_material] : grouped_shader.materials) {
                HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderGroupedMaterial");

                UseMaterial(opengl_shader, *grouped_material.material);

                for (const auto &[mesh_id, grouped_mesh] : grouped_material.meshes) {
                    HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderGroupedMesh");

                    const OpenGLMesh &opengl_mesh = *grouped_mesh.mesh;
                    glBindVertexArray(opengl_mesh.vertex_array);

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
        switch (attributes.blending_mode) 	{
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
        switch (attributes.culling_mode) 	{
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
    void OpenGLRenderDriver::DrawSubMesh(const SubMesh &sub_mesh) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.DrawSubMesh");

        void *index_offset = reinterpret_cast<void *>(static_cast<uint32>(sub_mesh.index_offset) * sizeof(uint32));

        m_stats.draw_calls += 1;
        m_stats.vertex_count += sub_mesh.vertex_count;
        m_stats.triangle_count += sub_mesh.index_count;

        GLenum topology = GetGLTopology(sub_mesh.topology);
        glDrawElementsBaseVertex(topology, sub_mesh.index_count, GL_UNSIGNED_INT, index_offset, sub_mesh.vertex_offset);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::DrawRenderBounds(const BoundingBox &bounds) {
        Color color = Color::Red();

        struct ImmediateVertex {
            Vector3 position;
            Color color;
        };

        Array<ImmediateVertex> data;
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

        if (m_state.render_bounds_vertex_buffer == UINT32_MAX) {
            glCreateBuffers(1, &m_state.render_bounds_vertex_buffer);
            glNamedBufferData(m_state.render_bounds_vertex_buffer, data.GetLength() * sizeof(ImmediateVertex), data.GetData(), GL_DYNAMIC_DRAW);

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
            glVertexArrayVertexBuffer(m_state.render_bounds_vertex_array, 0, m_state.render_bounds_vertex_buffer, 0, sizeof(ImmediateVertex));
            glVertexArrayElementBuffer(m_state.render_bounds_vertex_array, m_state.render_bounds_index_buffer);

            glEnableVertexArrayAttrib(m_state.render_bounds_vertex_array, 0);
            glVertexArrayAttribFormat(m_state.render_bounds_vertex_array, 0, 3, GL_FLOAT, false, 0);
            glVertexArrayAttribBinding(m_state.render_bounds_vertex_array, 0, 0);
            glEnableVertexArrayAttrib(m_state.render_bounds_vertex_array, 3);
            glVertexArrayAttribFormat(m_state.render_bounds_vertex_array, 3, 4, GL_FLOAT, false, sizeof(Vector3));
            glVertexArrayAttribBinding(m_state.render_bounds_vertex_array, 3, 0);
        } else {
            glNamedBufferSubData(m_state.render_bounds_vertex_buffer, 0, data.GetLength() * sizeof(ImmediateVertex), data.GetData());
        }

        glBindVertexArray(m_state.render_bounds_vertex_array);
        glDrawElementsBaseVertex(GL_LINES, 24, GL_UNSIGNED_INT, 0, 0);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::UnloadAssets(RenderFrameContext &render_frame_context) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.UnloadAssets");
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
        GLenum internal_format = GetTextureInternalFormat(format);
        GLsizei mipmap_count = texture_2d.parameters.attributes.use_mipmaps ? Math::Max(texture_2d.mipmap_count, 1) : 1;
        glTextureStorage2D(opengl_texture.texture, mipmap_count, internal_format, width, height);

        FlipTextureHorizontally(texture_2d.parameters.width, texture_2d.parameters.height, format, texture_2d.pixels);
        GLenum format_value = GetTextureFormat(format);
        GLenum format_type = GetTextureFormatType(format);
        SetUnpackAlignmentForTextureFormat(format);
        glTextureSubImage2D(opengl_texture.texture, 0, 0, 0, width, height, format_value, format_type, texture_2d.pixels.GetData());

        if (texture_2d.parameters.attributes.use_mipmaps) {
            glGenerateTextureMipmap(opengl_texture.texture);
        }

        m_opengl_textures.Insert(texture_2d.id, opengl_texture);
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
            GLuint attribute_index = GetGLAttributeIndexForVertextAttributeSize(vertex_attribute.kind);
            GLuint binding_index = 0;
            GLint size = vertex_attribute.dimension;
            GLenum type = GetGLVertexAttributeType(vertex_attribute.type);

            glEnableVertexArrayAttrib(opengl_mesh.vertex_array, attribute_index);
            glVertexArrayAttribFormat(opengl_mesh.vertex_array, attribute_index, size, type, false, relative_offset);
            glVertexArrayAttribBinding(opengl_mesh.vertex_array, attribute_index, binding_index);

            relative_offset += GetGLVertexAttributeSizeForVertexAttribute(vertex_attribute.type, vertex_attribute.dimension);
        }

        m_opengl_meshes.Insert(mesh.id, opengl_mesh);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::SetTextureAttributes(GLuint texture, TextureAttributes attributes) {
        GLint wrap_mode = GetTextureWrapMode(attributes.wrap_mode);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, wrap_mode);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, wrap_mode);

        GLint min_filter = GetTextureMinFilter(attributes.filter);
        GLint mag_filter = GetTextureMaxFilter(attributes.filter);
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, mag_filter);

        GLfloat anisotropic_filter_value = GetTextureAnisotropicFilter(attributes.anisotropic_filter);
        glTextureParameterf(texture, GL_TEXTURE_MAX_ANISOTROPY, anisotropic_filter_value);
    }

}