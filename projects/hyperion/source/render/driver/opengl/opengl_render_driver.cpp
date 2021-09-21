//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

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
            case Hyperion::Rendering::TextureFormat::RGB24: alignment = 4; break;
            case Hyperion::Rendering::TextureFormat::RGBA32: alignment = 4; break;
            case Hyperion::Rendering::TextureFormat::R8: alignment = 1; break;
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
            uint8 *source = data + (row * stride);
            uint8 *destination = data + (((height - 1) - row) * stride);

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

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
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

        const Array<RenderFrameCommand> &commands = render_frame->GetCommands();
        for (const RenderFrameCommand &command : commands) {
            switch (command.type) {
                case RenderFrameCommandType::SetCamera: {
                    HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.SetCamera");

                    m_state.camera_index = command.data.set_camera.camera_index;

                    break;
                }
                case RenderFrameCommandType::Clear: {
                    HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.Clear");

                    const RenderFrameContextCamera &render_frame_context_camera = render_frame_context.GetCameras()[m_state.camera_index];

                    const CameraViewport &viewport = render_frame_context_camera.viewport;
                    glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
                    Color background_color = render_frame_context_camera.background_color;
                    glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                    break;
                }
                case RenderFrameCommandType::DrawMeshes: {
                    HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.DrawMeshes");

                    const RenderFrameContextCamera &render_frame_context_camera = render_frame_context.GetCameras()[m_state.camera_index];

                    GroupObjects(render_frame_context.GetMeshObjects(), render_frame_context_camera.visibility_mask);
                    RenderCamera(render_frame_context.GetEnvironment(), render_frame_context.GetLights(), render_frame_context_camera);

                    break;
                }
                case RenderFrameCommandType::DrawGizmos: {
                    HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameCommand.DrawGizmos");

                    const RenderFrameContextCamera &render_frame_context_camera = render_frame_context.GetCameras()[m_state.camera_index];

                    const OpenGLShader &opengl_shader = m_opengl_shaders.Get(command.data.draw_gizmos.shader_id);
                    glUseProgram(opengl_shader.program);
                    glProgramUniformMatrix4fv(opengl_shader.program, glGetUniformLocation(opengl_shader.program, "u_view"), 1, GL_FALSE, render_frame_context_camera.view_matrix.elements);
                    glProgramUniformMatrix4fv(opengl_shader.program, glGetUniformLocation(opengl_shader.program, "u_projection"), 1, GL_FALSE, render_frame_context_camera.projection_matrix.elements);

                    if (command.data.draw_gizmos.grid.should_draw) {
                        glEnable(GL_BLEND);

                        GLint model_location = glGetUniformLocation(opengl_shader.program, "u_model");
                        glProgramUniformMatrix4fv(opengl_shader.program, model_location, 1, GL_FALSE, command.data.draw_gizmos.grid.local_to_world.elements);

                        const OpenGLMesh &opengl_mesh = m_opengl_meshes.Get(command.data.draw_gizmos.grid.mesh_id);
                        glBindVertexArray(opengl_mesh.vertex_array);

                        SubMesh sub_mesh = opengl_mesh.sub_meshes[0];
                        DrawSubMesh(sub_mesh);

                        glDisable(GL_BLEND);
                    }

                    break;
                }
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
            }
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::GroupObjects(const Array<RenderFrameContextObjectMesh> &mesh_objects, LayerMask visibility_mask) {
        HYP_PROFILE_CATEGORY("OpenGLRenderDriver.GroupObjects", ProfileCategory::Rendering);

        Array<GroupedShader> grouped_shaders;

        for (const RenderFrameContextObjectMesh &render_frame_context_object_mesh : mesh_objects) {
            if ((render_frame_context_object_mesh.layer_mask & visibility_mask) != render_frame_context_object_mesh.layer_mask) {
                continue;
            }

            AssetId material_id = render_frame_context_object_mesh.material->GetAssetInfo().id;
            AssetId shader_id = render_frame_context_object_mesh.material->GetShader()->GetAssetInfo().id;
            AssetId mesh_id = render_frame_context_object_mesh.mesh->GetAssetInfo().id;

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

            grouped_mesh->objects.Add(render_frame_context_object_mesh);
        }

        m_grouped_shaders = std::move(grouped_shaders);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::RenderCamera(const RenderFrameContextEnvironment &environment, const Array<RenderFrameContextLight> &lights, const RenderFrameContextCamera &camera) {
        HYP_PROFILE_CATEGORY("OpenGLRenderDriver.RenderCamera", ProfileCategory::Rendering);

        for (const GroupedShader &grouped_shader : m_grouped_shaders) {
            HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderGroupedShader");

            const OpenGLShader &opengl_shader = *grouped_shader.shader;
            glUseProgram(opengl_shader.program);
            glProgramUniformMatrix4fv(opengl_shader.program, glGetUniformLocation(opengl_shader.program, "u_view"), 1, GL_FALSE, camera.view_matrix.elements);
            glProgramUniformMatrix4fv(opengl_shader.program, glGetUniformLocation(opengl_shader.program, "u_projection"), 1, GL_FALSE, camera.projection_matrix.elements);

            Color ambient_color = environment.ambient_light.intensity * environment.ambient_light.color;
            glProgramUniform3f(opengl_shader.program, glGetUniformLocation(opengl_shader.program, "u_lighting.ambient_color"), ambient_color.r, ambient_color.g, ambient_color.b);

            // Because of how the frame context gets populated, we know that the first light (if present and directional) is the main light.
            auto light_it = lights.begin();
            if (light_it != lights.end() && light_it->type == LightType::Directional) {
                const RenderFrameContextLight &main_light = *light_it;
                glProgramUniform1f(opengl_shader.program, glGetUniformLocation(opengl_shader.program, "u_lighting.main_light.intensity"), main_light.intensity);
                glProgramUniform4f(opengl_shader.program, glGetUniformLocation(opengl_shader.program, "u_lighting.main_light.color"), main_light.color.r, main_light.color.g, main_light.color.b, main_light.color.a);
                glProgramUniform3f(opengl_shader.program, glGetUniformLocation(opengl_shader.program, "u_lighting.main_light.direction"), main_light.direction.x, main_light.direction.y, main_light.direction.z);
            }

            for (const auto [material_id, grouped_material] : grouped_shader.materials) {
                HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderGroupedMaterial");

                UseMaterial(opengl_shader, *grouped_material.material);

                for (const auto &[mesh_id, grouped_mesh] : grouped_material.meshes) {
                    HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderGroupedMesh");

                    const OpenGLMesh &opengl_mesh = *grouped_mesh.mesh;
                    glBindVertexArray(opengl_mesh.vertex_array);

                    GLint model_location = glGetUniformLocation(opengl_shader.program, "u_model");
                    for (const RenderFrameContextObjectMesh &render_frame_context_mesh_object : grouped_mesh.objects) {
                        HYP_PROFILE_SCOPE("OpenGLRenderDriver.RenderFrameMeshObject");
                        
                        glProgramUniformMatrix4fv(opengl_shader.program, model_location, 1, GL_FALSE, render_frame_context_mesh_object.local_to_world.elements);

                        SubMesh sub_mesh = opengl_mesh.sub_meshes[render_frame_context_mesh_object.sub_mesh_index];
                        DrawSubMesh(sub_mesh);
                    }
                }
            }
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::UseMaterial(const OpenGLShader &opengl_shader, const OpenGLMaterial &opengl_material) {
        HYP_PROFILE_SCOPE("OpenGLRenderDriver.UseMaterial");

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
        opengl_shader.program = OpenGLRenderDriverShaderCompiler::Compile(shader.data.vertex_source.c_str(), shader.data.fragment_source.c_str()).program;

        opengl_shader.locations.Resize(shader.data.properties.GetLength());
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
        Array<uint8> vertices(vertex_count * vertex_format.stride);
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