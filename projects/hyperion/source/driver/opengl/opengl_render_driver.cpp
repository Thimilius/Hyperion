//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/driver/opengl/opengl_render_driver.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/io/file_system.hpp"
#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"
#include "hyperion/driver/opengl/opengl_utilities.hpp"
#include "hyperion/rendering/shaders/shader_pre_processor.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    // NOTE: So here is the thing...
    // Because the actual creation and initialization of graphics resources gets delayed when using a Render Thread,
    // it is possible to query buffer, texture and other data referencing a resource that is not yet actually created.
    // For now we handle this by returning empty data in the appropriate functions.
    // A better architecture in general would probably be to lazy initialize the graphics resources when creating them from the Main Thread.
    // The actual initialization of the resources can then be executed later on the Render Thread.
    // This would require quite some work though and the creation would probably need to be synchronized somehow.

    //--------------------------------------------------------------
    void OpenGLRenderDriver::Initialize(GraphicsContext *graphics_context) {
        m_graphics_context = static_cast<OpenGLGraphicsContext *>(graphics_context);

        // FIXME: Currently we are not cleaning up those resources properly.
        {
            ShaderPreProcessor pre_processor(FileSystem::ReadAllText("data/shaders/internal/fallback.shader"));
            ShaderPreProcessResult pre_process_result = pre_processor.PreProcess();
            HYP_ASSERT(pre_process_result.success);
            Map<ShaderStageFlags, String> &sources = pre_process_result.sources;
            OpenGLShaderCompilationResult compilation_result = OpenGLShaderCompiler::Compile(sources[ShaderStageFlags::Vertex].c_str(), sources[ShaderStageFlags::Fragment].c_str());
            HYP_ASSERT(compilation_result.success);
            m_fallback_shader = compilation_result.program;

            m_fallback_material.shader_id = 0;
            CollectMaterialProperties(m_fallback_material, m_fallback_shader);
        }
        {
            ShaderPreProcessor pre_processor(FileSystem::ReadAllText("data/shaders/internal/fullscreen.shader"));
            ShaderPreProcessResult pre_process_result = pre_processor.PreProcess();
            HYP_ASSERT(pre_process_result.success);
            Map<ShaderStageFlags, String> &sources = pre_process_result.sources;
            OpenGLShaderCompilationResult compilation_result = OpenGLShaderCompiler::Compile(sources[ShaderStageFlags::Vertex].c_str(), sources[ShaderStageFlags::Fragment].c_str());
            HYP_ASSERT(compilation_result.success);
            m_fullscreen_shader = compilation_result.program;
            // We always need a vertex array to draw anything (in this case a fullscreen triangle).
            // It does not need to contain anything and can therefore be empty.
            glCreateVertexArrays(1, &m_fullscreen_vao);
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::Shutdown() {
        glDeleteShader(m_fallback_shader);
        glDeleteShader(m_fullscreen_shader);
        glDeleteVertexArrays(1, &m_fullscreen_vao);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::Clear(ClearFlags clear_flags, Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(OpenGLUtilities::GetGLClearFlags(clear_flags));
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::SetViewport(const Viewport &viewport) {
        glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::SetRasterizerState(const RasterizerState &rasterizer_state) {
        // NOTE: Currently we are setting the whole rasterizer state at once, regardless of wether or not we actually have new values we need to change.
        // TLDR: This is very expensive.

        // Depth
        {
            if (rasterizer_state.depth_test_enabled) {
                glEnable(GL_DEPTH_TEST);
            } else {
                glDisable(GL_DEPTH_TEST);
            }
            if (rasterizer_state.depth_mask_enabled) {
                glDepthMask(GL_TRUE);
            } else {
                glDepthMask(GL_FALSE);
            }
            glDepthFunc(OpenGLUtilities::GetGLDepthEquation(rasterizer_state.depth_equation));
        }

        // Stencil
        {
            if (rasterizer_state.stencil_test_enabled) {
                glEnable(GL_STENCIL_TEST);
            } else {
                glDisable(GL_STENCIL_TEST);
            }
        }

        // Blending
        {
            if (rasterizer_state.blending_enabled) {
                glEnable(GL_BLEND);
            } else {
                glDisable(GL_BLEND);
            }
            GLuint source_factor = OpenGLUtilities::GetGLBlendingFactor(rasterizer_state.blending_function.source_factor);
            GLuint destination_factor = OpenGLUtilities::GetGLBlendingFactor(rasterizer_state.blending_function.destination_factor);
            glBlendFunc(source_factor, destination_factor);
            glBlendEquation(OpenGLUtilities::GetGLBlendingEquation(rasterizer_state.blending_equation));
        }

        // Culling
        {
            if (rasterizer_state.culling_enabled) {
                glEnable(GL_CULL_FACE);
            } else {
                glDisable(GL_CULL_FACE);
            }
            glCullFace(OpenGLUtilities::GetGLCullingMode(rasterizer_state.culling_mode));
            glFrontFace(OpenGLUtilities::GetGLCullingFrontFaceMode(rasterizer_state.culling_front_face_mode));
        }

        // Misc
        {
            glPolygonMode(GL_FRONT_AND_BACK, OpenGLUtilities::GetGLPolygonMode(rasterizer_state.polygon_mode));
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::SetCameraData(const CameraData &camera_data) {
        m_current_camera_data = camera_data;

        // When setting a new camera data we want to invalidate the currently used material.
        // This way we know we will set the new camera state to the currently used material when doing the next draw.
        // TODO: This can and should be avoided when using uniform buffers.
        m_current_material = nullptr;
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::CreateShader(ResourceId shader_id, const ShaderDescriptor &descriptor) {
        HYP_ASSERT(m_shaders.find(shader_id) == m_shaders.end());

        OpenGLShader &shader = m_shaders[shader_id];
        if (descriptor.use_fallback) {
            shader.program = m_fallback_shader;
        } else {
            OpenGLShaderCompilationResult compilation_result = OpenGLShaderCompiler::Compile(descriptor.source_vertex.data, descriptor.source_fragment.data);
            if (compilation_result.success) {
                shader.program = compilation_result.program;
            } else {
                shader.program = m_fallback_shader;
            }
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::RecompileShader(ResourceId shader_id, const ShaderDescriptor &descriptor) {
        HYP_ASSERT(m_shaders.find(shader_id) != m_shaders.end());

        DestroyShader(shader_id);
        CreateShader(shader_id, descriptor);

        // Recompiling a shader actually involves not only recreating the actual shader but also updating all materials that use the shader.
        for (auto &[material_id, material] : m_materials) {
            if (material.shader_id == shader_id) {
                material.properties.clear();
                material.textures.clear();

                HYP_ASSERT(m_shaders.find(shader_id) != m_shaders.end());
                OpenGLShader &shader = m_shaders[shader_id];

                CollectMaterialProperties(material, shader.program);
            }
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::DestroyShader(ResourceId id) {
        HYP_ASSERT(m_shaders.find(id) != m_shaders.end());

        OpenGLShader &shader = m_shaders[id];

        // NOTE: We have to remember the special case where the shader might point to the fallback shader which we DO NOT want to delete.
        if (shader.program != m_fallback_shader) {
            glDeleteProgram(shader.program);
        }
        
        m_shaders.erase(id);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::CreateTexture(ResourceId texture_id, const TextureDescriptor &descriptor) {
        HYP_ASSERT(m_textures.find(texture_id) == m_textures.end());

        HYP_ASSERT(descriptor.size.width > 0 && descriptor.size.width <= m_graphics_context->GetLimits().max_texture_size);
        HYP_ASSERT(descriptor.size.height > 0 && descriptor.size.height <= m_graphics_context->GetLimits().max_texture_size);

        OpenGLTexture &texture = m_textures[texture_id];
        texture.dimension = descriptor.dimension;
        texture.format = descriptor.format;
        texture.parameters = descriptor.parameters;
        texture.size = descriptor.size;

        switch (descriptor.dimension) {
            case TextureDimension::Texture2D: CreateTexture2D(texture, descriptor); break;
            case TextureDimension::TextureCubemap: CreateTextureCubemap(texture, descriptor); break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::DestroyTexture(ResourceId texture_id) {
        HYP_ASSERT(m_textures.find(texture_id) != m_textures.end());

        OpenGLTexture &texture = m_textures[texture_id];
        glDeleteTextures(1, &texture.texture);

        m_textures.erase(texture_id);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::CreateMaterial(ResourceId material_id, const MaterialDescriptor &descriptor) {
        HYP_ASSERT(m_materials.find(material_id) == m_materials.end());

        OpenGLMaterial &material = m_materials[material_id];
        ResourceId shader_id = descriptor.shader_id;
        HYP_ASSERT(m_shaders.find(shader_id) != m_shaders.end());
        material.id = material_id;
        material.shader_id = shader_id;

        OpenGLShader &shader = m_shaders[shader_id];
        CollectMaterialProperties(material, shader.program);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::SetMaterialProperty(ResourceId material_id, const MaterialProperty &property) {
        HYP_ASSERT(m_materials.find(material_id) != m_materials.end());
        OpenGLMaterial &material = m_materials[material_id];
        
        ResourceId shader_id = material.shader_id;
        HYP_ASSERT(m_shaders.find(shader_id) != m_shaders.end());
        OpenGLShader &shader = m_shaders[shader_id];
        GLuint program = shader.program;

        MaterialPropertyId property_id = property.id;
        auto it = material.properties.find(property_id);
        if (it == material.properties.end()) {
            // We simply ignore a property if it does not actually exist on the shader.
            return;
        }
        if (it->second.property.type != property.type) {
            // Textures and render textures are actually compatible with each other.
            bool is_compatible_texture = (it->second.property.type == MaterialPropertyType::RenderTexture || property.type == MaterialPropertyType::RenderTexture) &&
                (it->second.property.type == MaterialPropertyType::Texture || property.type == MaterialPropertyType::Texture);
            if (!is_compatible_texture) {
                HYP_LOG_ERROR("OpenGL", "Trying to assign material property '{}' a wrong type!", it->second.name);
                return;
            }
        }

        it->second.property.storage = property.storage;
        GLint location = it->second.location;

        switch (property.type) {
            case MaterialPropertyType::Float32: {
                glProgramUniform1f(program, location, property.storage.float32);
                break;
            }
            case MaterialPropertyType::Int32: {
                glProgramUniform1i(program, location, property.storage.int32);
                break;
            }
            case MaterialPropertyType::UInt32: {
                glProgramUniform1ui(program, location, property.storage.unsigned_int32);
                break;
            }
            case MaterialPropertyType::Vec2: {
                Vec2 vec2 = property.storage.vec2;
                glProgramUniform2f(program, location, vec2.x, vec2.y);
                break;
            }
            case MaterialPropertyType::Vec3: {
                Vec3 vec3 = property.storage.vec3;
                glProgramUniform3f(program, location, vec3.x, vec3.y, vec3.z);
                break;
            }
            case MaterialPropertyType::Vec4: {
                Vec4 vec4 = property.storage.vec4;
                glProgramUniform4f(program, location, vec4.x, vec4.y, vec4.z, vec4.w);
                break;
            }
            case MaterialPropertyType::Mat3: {
                glProgramUniformMatrix3fv(program, location, 1, GL_FALSE, property.storage.mat3.elements);
                break;
            }
            case MaterialPropertyType::Mat4: {
                glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, property.storage.mat4.elements);
                break;
            }
            case MaterialPropertyType::Texture:
            case MaterialPropertyType::RenderTexture: {
                // A material stores a normal texture and a render texture in the same way.
                GLuint texture_object = 0;
                if (property.type == MaterialPropertyType::Texture) {
                    ResourceId texture_id = property.storage.texture;
                    if (texture_id != 0) {
                        HYP_ASSERT(m_textures.find(texture_id) != m_textures.end());
                        OpenGLTexture &texture = m_textures[texture_id];
                        texture_object = texture.texture;
                    }
                    
                } else {
                    ResourceId render_texture_id = property.storage.render_texture.render_texture;
                    HYP_ASSERT(m_render_textures.find(render_texture_id) != m_render_textures.end());
                    OpenGLRenderTexture &render_texture = m_render_textures[render_texture_id];
                    HYP_ASSERT(property.storage.render_texture.attachment_index < render_texture.attachments.size());
                    OpenGLRenderTextureAttachment &attachment = render_texture.attachments[property.storage.render_texture.attachment_index];
                    HYP_ASSERT(attachment.attributes.format != RenderTextureFormat::Depth24Stencil8);
                    texture_object = attachment.attachment;
                }

                // We have to check if the property might already be set.
                auto &it = material.textures.find(property_id);
                if (it != material.textures.end()) {
                    it->second = texture_object;
                } else {
                    GLuint texture_unit = static_cast<GLuint>(material.textures.size());
                    HYP_ASSERT(texture_unit < static_cast<GLuint>(m_graphics_context->GetLimits().max_texture_units));
                    glProgramUniform1i(program, location, texture_unit);
                    material.textures[property_id] = texture_object;
                }
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::DestroyMaterial(ResourceId material_id) {
        HYP_ASSERT(m_materials.find(material_id) != m_materials.end());
        m_materials.erase(material_id);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::CreateRenderTexture(ResourceId render_texture_id, const RenderTextureDescriptor &descriptor) {
        HYP_ASSERT(m_render_textures.find(render_texture_id) == m_render_textures.end());

        HYP_ASSERT(descriptor.size.width > 0 && descriptor.size.width <= m_graphics_context->GetLimits().max_framebuffer_width);
        HYP_ASSERT(descriptor.size.height > 0 && descriptor.size.height <= m_graphics_context->GetLimits().max_framebuffer_height);

        OpenGLRenderTexture &render_texture = m_render_textures[render_texture_id];
        render_texture.id = render_texture_id;
        render_texture.size = descriptor.size;
        render_texture.mipmap_count = descriptor.mipmap_count;

        glCreateFramebuffers(1, &render_texture.render_texture);
        
        uint64 render_texture_attachment_count = descriptor.attachments.size / sizeof(descriptor.attachments.data[0]);
        GLenum color_attachment_index = 0;
        bool has_created_depth_stencil_attachment = false;
        for (uint32 i = 0; i < render_texture_attachment_count; i++) {
            const RenderTextureAttachment &render_texture_attachment = descriptor.attachments.data[i];

            OpenGLRenderTextureAttachment attachment = { };
            attachment.attributes.format = render_texture_attachment.format;
            attachment.attributes.parameters = render_texture_attachment.parameters;

            // TODO: We should have the ability to specify that we want to create a texture even for depth/stencil so that we can read from it in a shader.
            if (render_texture_attachment.format == RenderTextureFormat::Depth24Stencil8) {
                if (has_created_depth_stencil_attachment) {
                    HYP_LOG_ERROR("OpenGL", "Trying to add more than one depth and stencil attachment to a render texture!");
                    continue;
                }
                has_created_depth_stencil_attachment = true;

                glCreateRenderbuffers(1, &attachment.attachment);
                glNamedRenderbufferStorage(attachment.attachment, GL_DEPTH24_STENCIL8, render_texture.size.width, render_texture.size.height);
                glNamedFramebufferRenderbuffer(render_texture.render_texture, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, attachment.attachment);
            } else {
                glCreateTextures(GL_TEXTURE_2D, 1, &attachment.attachment);

                TextureParameters parameters = render_texture_attachment.parameters;
                SetTextureParameters(attachment.attachment, parameters);

                RenderTextureFormat format = render_texture_attachment.format;
                GLsizei width = descriptor.size.width;
                GLsizei height = descriptor.size.height;
                GLenum internal_format = OpenGLUtilities::GetGLRenderTextureInternalFormat(format);
                GLsizei mipmap_count = parameters.use_mipmaps ? Math::Max(descriptor.mipmap_count, 1) : 1;
                glTextureStorage2D(attachment.attachment, mipmap_count, internal_format, width, height);

                GLenum attachment_index = GL_COLOR_ATTACHMENT0 + color_attachment_index;
                HYP_ASSERT(color_attachment_index < m_graphics_context->GetLimits().max_framebuffer_color_attachments);
                glNamedFramebufferTexture(render_texture.render_texture, attachment_index, attachment.attachment, 0);
                color_attachment_index++;
            }

            render_texture.attachments.push_back(attachment);
        }

        render_texture.color_attachment_count = color_attachment_index;

        if (glCheckNamedFramebufferStatus(render_texture.render_texture, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            HYP_LOG_ERROR("OpenGL", "Failed to create framebuffer!");
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::ResizeRenderTexture(ResourceId render_texture_id, uint32 width, uint32 height, uint32 mipmap_count) {
        HYP_ASSERT(m_render_textures.find(render_texture_id) != m_render_textures.end());

        // We take an explicit copy of the render texture here, because a reference would point to a destroyed object later.
        OpenGLRenderTexture render_texture = m_render_textures[render_texture_id];
        if (render_texture.size.width != width || render_texture.size.height != height) {
            // We first destroy the old render texture and then create a new one with the properties of the old.
            // Not including the size and mipmap count of course.
            DestroyRenderTexture(render_texture_id);

            RenderTextureDescriptor descriptor = { };
            descriptor.mipmap_count = mipmap_count;
            descriptor.size = { width, height };
            Vector<RenderTextureAttachment> attachments;
            attachments.reserve(render_texture.attachments.size());
            for (OpenGLRenderTextureAttachment attachment : render_texture.attachments) {
                attachments.push_back(attachment.attributes);
            }
            descriptor.attachments = attachments;

            CreateRenderTexture(render_texture_id, descriptor);
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::GetRenderTextureSubData(ResourceId render_texture_id, uint32 attachment_index, RectInt region, Vector<uint8> *buffer, GetRenderTextureSubDataCallback callback) {
        auto it = m_render_textures.find(render_texture_id);
        if (it != m_render_textures.end()) {
            OpenGLRenderTexture &render_texture = it->second;
            HYP_ASSERT(attachment_index < render_texture.attachments.size());

            OpenGLRenderTextureAttachment &attachment = render_texture.attachments[attachment_index];
            RenderTextureFormat format = attachment.attributes.format;
            // NOTE: We currently do not allow to get the data of a non color attachment.
            HYP_ASSERT(format != RenderTextureFormat::Depth24Stencil8);

            // Make sure we are not out of bounds when accessing the render texture.
            bool x_range_is_not_valid = region.x < 0 || (region.width + region.x) < 0 || (region.width + region.x) > static_cast<int32>(render_texture.size.width);
            bool y_range_is_not_valid = region.y < 0 || (region.height + region.y) < 0 || (region.height + region.y) > static_cast<int32>(render_texture.size.height);
            if (x_range_is_not_valid || y_range_is_not_valid) {
                HYP_LOG_ERROR("OpenGL", "Trying to read out-of-bounds data of a render texture!");
                goto finish;
            }

            // We want to make sure that the buffer we got passed does actually have enough space.
            GLsizei buffer_size = OpenGLUtilities::GetRenderTextureBufferSize(region, format);
            buffer->resize(buffer_size);

            GLenum format_value = OpenGLUtilities::GetGLRenderTextureFormat(format);
            GLenum format_type = OpenGLUtilities::GetGLRenderTextureFormatType(format);

            glGetTextureSubImage(attachment.attachment, 0, region.x, region.y, 0, region.width, region.height, 1, format_value, format_type, buffer_size, buffer->data());
        }
         
    finish:
        if (callback != nullptr) {
            callback(buffer);
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::SetRenderTexture(ResourceId render_texture_id) {
        // We might get passed a 0, meaning the default render texture.
        if (render_texture_id == 0) {
            m_current_render_texture = nullptr;

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        } else {
            HYP_ASSERT(m_render_textures.find(render_texture_id) != m_render_textures.end());
            OpenGLRenderTexture &render_texture = m_render_textures[render_texture_id];

            if (m_current_render_texture != &render_texture) {
                m_current_render_texture = &render_texture;

                // We have to specify that we want to draw into all color attachments of the render texture.          
                uint32 color_attachment_count = render_texture.color_attachment_count;
                Vector<GLenum> buffers(color_attachment_count);
                for (GLenum i = 0; i < color_attachment_count; i++) {
                    buffers[i] = GL_COLOR_ATTACHMENT0 + i;
                }
                glNamedFramebufferDrawBuffers(render_texture.render_texture, color_attachment_count, buffers.data());

                glBindFramebuffer(GL_FRAMEBUFFER, render_texture.render_texture);
            }
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::BlitRenderTexture(ResourceId destination_id, ResourceId source_id) {
        // NOTE: We currently do not allow blitting from the default render texture to another render texture.
        HYP_ASSERT(source_id != 0);
        
        GLuint destination_render_texture = 0;
        if (destination_id != 0) {
            HYP_ASSERT(m_render_textures.find(destination_id) != m_render_textures.end());
            OpenGLRenderTexture &render_texture = m_render_textures[destination_id];
            destination_render_texture = render_texture.render_texture;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, destination_render_texture);

        HYP_ASSERT(m_render_textures.find(source_id) != m_render_textures.end());
        OpenGLRenderTexture &source_render_texture = m_render_textures[source_id];
        HYP_ASSERT(source_render_texture.color_attachment_count >= 1 && source_render_texture.attachments[0].attributes.format != RenderTextureFormat::Depth24Stencil8);

        m_current_material = nullptr;
        glUseProgram(m_fullscreen_shader);
        glBindTextureUnit(0, source_render_texture.attachments[0].attachment);
        glBindVertexArray(m_fullscreen_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        if (m_current_render_texture != nullptr) {
            ResourceId current_render_texture_id = m_current_render_texture->id;
            m_current_render_texture = nullptr;
            SetRenderTexture(current_render_texture_id);
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::DestroyRenderTexture(ResourceId render_texture_id) {
        HYP_ASSERT(m_render_textures.find(render_texture_id) != m_render_textures.end());

        OpenGLRenderTexture &render_texture = m_render_textures[render_texture_id];
        glDeleteFramebuffers(1, &render_texture.render_texture);
        for (const OpenGLRenderTextureAttachment &attachment : render_texture.attachments) {
            if (attachment.attributes.format == RenderTextureFormat::Depth24Stencil8) {
                glDeleteRenderbuffers(1, &attachment.attachment);
            } else {
                glDeleteTextures(1, &attachment.attachment);
            }
        }

        m_render_textures.erase(render_texture_id);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::CreateMesh(ResourceId mesh_id, const MeshDescriptor &descriptor) {
        HYP_ASSERT(m_meshes.find(mesh_id) == m_meshes.end());

        OpenGLMesh &mesh = m_meshes[mesh_id];
        mesh.index_format = descriptor.index_format;
        mesh.sub_meshes.assign(descriptor.sub_meshes.data, descriptor.sub_meshes.data + (descriptor.sub_meshes.size / sizeof(*descriptor.sub_meshes.data)));

        // TODO: Add ability to mark meshes dynamic that get updated frequently.
        glCreateBuffers(2, &mesh.vertex_buffer);
        glNamedBufferData(mesh.vertex_buffer, descriptor.vertices.size, descriptor.vertices.data, GL_STATIC_DRAW);
        glNamedBufferData(mesh.index_buffer, descriptor.indices.size, descriptor.indices.data, GL_STATIC_DRAW);

        glCreateVertexArrays(1, &mesh.vertex_array);
        GLuint vertex_array_id = mesh.vertex_array;

        glVertexArrayElementBuffer(vertex_array_id, mesh.index_buffer);

        GLsizei stride = descriptor.vertex_format.stride;
        GLuint relative_offset = 0;
        uint64 vertex_attribute_count = descriptor.vertex_format.attributes.size / sizeof(descriptor.vertex_format.attributes.data[0]);
        for (uint32 i = 0; i < vertex_attribute_count; i++) {
            const VertexAttribute &vertex_attribute = descriptor.vertex_format.attributes.data[i];
            
            GLuint attribute_index = OpenGLUtilities::GetGLAttributeIndexForVertextAttributeSize(vertex_attribute.kind);
            GLuint binding_index = 0;
            GLint size = vertex_attribute.dimension;
            GLenum type = OpenGLUtilities::GetGLVertexAttributeType(vertex_attribute.type);

            glEnableVertexArrayAttrib(vertex_array_id, attribute_index);
            glVertexArrayAttribFormat(vertex_array_id, attribute_index, size, type, false, relative_offset);
            glVertexArrayVertexBuffer(vertex_array_id, binding_index, mesh.vertex_buffer, 0, stride);
            glVertexArrayAttribBinding(vertex_array_id, attribute_index, binding_index);

            relative_offset += OpenGLUtilities::GetGLVertexAttributeSizeForVertexAttribute(vertex_attribute.type, vertex_attribute.dimension);
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::SetMeshData(ResourceId mesh_id, const MeshDataDescriptor &descriptor) {
        HYP_ASSERT(m_meshes.find(mesh_id) != m_meshes.end());
        OpenGLMesh &mesh = m_meshes[mesh_id];

        // NOTE: Do we always want to update both vertices and indices?
        glNamedBufferSubData(mesh.vertex_buffer, 0, descriptor.vertices.size, descriptor.vertices.data);
        glNamedBufferSubData(mesh.index_buffer, 0, descriptor.indices.size, descriptor.indices.data);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::DrawMesh(ResourceId mesh_id, const Mat4 &model_matrix, ResourceId material_id, uint32 sub_mesh_index) {
        // We are going to be very error tolerant here when drawing a mesh.

        auto material_it = m_materials.find(material_id);
        if (material_it != m_materials.end()) {
            OpenGLMaterial &material = material_it->second;

            ResourceId shader_id = material.shader_id;
            HYP_ASSERT(m_shaders.find(shader_id) != m_shaders.end());
            OpenGLShader &shader = m_shaders[shader_id];

            UseMaterial(material, shader.program, model_matrix);
        } else {
            UseMaterial(m_fallback_material, m_fallback_shader, model_matrix);
        }

        auto mesh_it = m_meshes.find(mesh_id);
        if (mesh_it != m_meshes.end()) {
            OpenGLMesh &mesh = mesh_it->second;
            if (sub_mesh_index < mesh.sub_meshes.size()) {
                SubMesh &sub_mesh = mesh.sub_meshes[sub_mesh_index];

                GLenum index_format = OpenGLUtilities::GetGLIndexFormat(mesh.index_format);
                GLsizei index_format_size = OpenGLUtilities::GetGLIndexFormatSize(mesh.index_format);
                const void *offset = reinterpret_cast<const void *>(static_cast<uint64>(sub_mesh.index_offset) * index_format_size);
                glBindVertexArray(mesh.vertex_array);
                glDrawElementsBaseVertex(OpenGLUtilities::GetGLMeshTopology(sub_mesh.topology), sub_mesh.index_count, index_format, offset, sub_mesh.vertex_offset);
            }
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::DestroyMesh(ResourceId mesh_id) {
        HYP_ASSERT(m_meshes.find(mesh_id) != m_meshes.end());

        OpenGLMesh &mesh = m_meshes[mesh_id];
        glDeleteBuffers(2, &mesh.vertex_buffer);
        glDeleteVertexArrays(1, &mesh.vertex_array);

        m_meshes.erase(mesh_id);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::CreateTexture2D(OpenGLTexture &texture, const TextureDescriptor &descriptor) {
        glCreateTextures(GL_TEXTURE_2D, 1, &texture.texture);
        GLuint texture_id = texture.texture;

        TextureParameters parameters = descriptor.parameters;
        SetTextureParameters(texture_id, parameters);

        TextureFormat format = descriptor.format;
        GLsizei width = descriptor.size.width;
        GLsizei height = descriptor.size.height;
        GLenum internal_format = OpenGLUtilities::GetGLTextureInternalFormat(format);
        GLsizei mipmap_count = parameters.use_mipmaps ? Math::Max(descriptor.mipmap_count, 1) : 1;
        glTextureStorage2D(texture_id, mipmap_count, internal_format, width, height);

        if (descriptor.pixels.size > 0) {
            OpenGLUtilities::FlipTextureHorizontally(descriptor.size, format, descriptor.pixels);
            
            GLenum format_value = OpenGLUtilities::GetGLTextureFormat(format);
            GLenum format_type = OpenGLUtilities::GetGLTextureFormatType(format);
            OpenGLUtilities::SetUnpackAlignmentForTextureFormat(descriptor.format);
            glTextureSubImage2D(texture_id, 0, 0, 0, width, height, format_value, format_type, descriptor.pixels.data);

            if (parameters.use_mipmaps) {
                glGenerateTextureMipmap(texture_id);
            }
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::CreateTextureCubemap(OpenGLTexture &texture, const TextureDescriptor &descriptor) {
        // TODO: Implement.
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::SetTextureParameters(GLuint texture, const TextureParameters &parameters) {
        GLint wrap_mode = OpenGLUtilities::GetGLTextureWrapMode(parameters.wrap_mode);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, wrap_mode);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, wrap_mode);

        GLint min_filter = OpenGLUtilities::GetGLTextureMinFilter(parameters.filter);
        GLint mag_filter = OpenGLUtilities::GetGLTextureMaxFilter(parameters.filter);
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, mag_filter);

        GLfloat anisotropic_filter_value = OpenGLUtilities::GetGLTextureAnisotropicFilter(parameters.anisotropic_filter);
        glTextureParameterf(texture, GL_TEXTURE_MAX_ANISOTROPY, anisotropic_filter_value);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::CollectMaterialProperties(OpenGLMaterial &material, GLuint program) {
        GLint active_uniform_count;
        glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &active_uniform_count);

        for (GLint uniform_index = 0; uniform_index < active_uniform_count; uniform_index++) {
            const uint64 NAME_BUFFER_SIZE = 64;
            String name_buffer;
            name_buffer.resize(NAME_BUFFER_SIZE);
            GLsizei name_length;
            glGetProgramResourceName(program, GL_UNIFORM, uniform_index, NAME_BUFFER_SIZE, &name_length, name_buffer.data());
            name_buffer.resize(name_length);

            MaterialPropertyId material_id = MaterialProperty::NameToId(name_buffer);
            HYP_ASSERT(material.properties.find(material_id) == material.properties.end());
            GLint location = glGetProgramResourceLocation(program, GL_UNIFORM, name_buffer.c_str());
            if (location < 0) {
                HYP_LOG_ERROR("OpenGL", "Failed to get location for active uniform: '{}'!", name_buffer);
            } else {
                GLenum resource_property = GL_TYPE;
                GLint type;
                glGetProgramResourceiv(program, GL_UNIFORM, uniform_index, 1, &resource_property, 1, nullptr, &type);

                OpenGLMaterialProperty &property = material.properties[material_id];
                property.property.id = MaterialProperty::NameToId(name_buffer);
                property.property.type = OpenGLUtilities::GetMaterialPropertyTypeForGLShaderType(type);
                property.name = name_buffer;
                property.location = location;

                SetMaterialPropertyToDefault(property);
            }
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::SetMaterialPropertyToDefault(OpenGLMaterialProperty &property) {
        switch (property.property.type) 	{
            case MaterialPropertyType::Float32: property.property.storage.float32 = 0.0f; break;
            case MaterialPropertyType::Int32: property.property.storage.int32 = 0; break;
            case MaterialPropertyType::UInt32: property.property.storage.unsigned_int32 = 0; break;
            case MaterialPropertyType::Vec2: property.property.storage.vec2 = Vec2(0.0f, 0.0f); break;
            case MaterialPropertyType::Vec3: property.property.storage.vec3 = Vec3(0.0f, 0.0f, 0.0f); break;
            case MaterialPropertyType::Vec4: property.property.storage.vec4 = Vec4(0.0f, 0.0f, 0.0f, 0.0f); break;
            case MaterialPropertyType::Mat3: property.property.storage.mat3 = Mat3(1.0f); break;
            case MaterialPropertyType::Mat4: property.property.storage.mat4 = Mat4::Identity(); break;
            case MaterialPropertyType::Texture: property.property.storage.texture = 0; break;
            case MaterialPropertyType::RenderTexture: property.property.storage.render_texture.render_texture = 0; property.property.storage.render_texture.attachment_index = 0; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::UseMaterial(OpenGLMaterial &material, GLuint program, const Mat4 &model_matrix) {
        if (m_current_material != &material) {
            m_current_material = &material;

            glUseProgram(program);

            for (auto &[property_id, property] : material.properties) {
                SetMaterialProperty(material.id, property.property);
            }

            {
                static const MaterialPropertyId PROJECTION_TRANSFORM_PROPERTY_ID = MaterialProperty::NameToId("u_transform.projection");
                HYP_ASSERT(material.properties.find(PROJECTION_TRANSFORM_PROPERTY_ID) != material.properties.end());
                OpenGLMaterialProperty &property = material.properties[PROJECTION_TRANSFORM_PROPERTY_ID];
                GLint location = property.location;
                glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, m_current_camera_data.projection_matrix.elements);
            }
            {
                static const MaterialPropertyId VIEW_TRANSFORM_PROPERTY_ID = MaterialProperty::NameToId("u_transform.view");
                HYP_ASSERT(material.properties.find(VIEW_TRANSFORM_PROPERTY_ID) != material.properties.end());
                OpenGLMaterialProperty &property = material.properties[VIEW_TRANSFORM_PROPERTY_ID];
                GLint location = property.location;
                glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, m_current_camera_data.view_matrix.elements);
            }
        }

        // We will always need to set the transformation matrix as well as binding the correct textures.

        GLuint texture_unit = 0;
        for (auto [property_id, texture] : material.textures) {
            glBindTextureUnit(texture_unit, texture);
            texture_unit++;
        }

        // Currently we are going to assume that every shader has this specific property.
        // TODO: Maybe we can cache that special property in the material directly.
        static const MaterialPropertyId MODEL_TRANSFORM_PROPERTY_ID = MaterialProperty::NameToId("u_transform.model");
        HYP_ASSERT(material.properties.find(MODEL_TRANSFORM_PROPERTY_ID) != material.properties.end());
        OpenGLMaterialProperty &property = material.properties[MODEL_TRANSFORM_PROPERTY_ID];
        GLint location = property.location;
        glProgramUniformMatrix4fv(program, location, 1, GL_FALSE, model_matrix.elements);
    }

}