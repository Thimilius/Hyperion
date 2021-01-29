#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_render_driver.hpp"

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"
#include "hyperion/driver/opengl/opengl_utilities.hpp"

namespace Hyperion::Rendering {

    void OpenGLRenderDriver::Initialize(GraphicsContext *graphics_context) {
        m_graphics_context = static_cast<OpenGLGraphicsContext *>(graphics_context);
    }

    void OpenGLRenderDriver::Clear(ClearFlags clear_flags, Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(OpenGLUtilities::GetGLClearFlags(clear_flags));
    }

    void OpenGLRenderDriver::SetViewport(const Viewport &viewport) {
        glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
    }

    void OpenGLRenderDriver::SetRasterizerState(const RasterizerState &rasterizer_state) {
        // NOTE: Currently we are setting the whole rasterizer state at once,
        // regardless of wether or not we actually have new values we want to change.
        // So this is very expensive.

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

    void OpenGLRenderDriver::CreateShader(ResourceId id, const ShaderDescriptor &descriptor) {
        HYP_ASSERT(s_shaders.find(id) == s_shaders.end());

        OpenGLShaderCompilationResult compilation_result = OpenGLShaderCompiler::Compile(descriptor.source_vertex.data, descriptor.source_fragment.data);
        if (compilation_result.succes) {
            OpenGLShader &shader = s_shaders[id];
            shader.program = compilation_result.program;
        } else {
            // TODO: How do we handle shader errors here?
        }
    }

    void OpenGLRenderDriver::DestroyShader(ResourceId id) {
        HYP_ASSERT(s_shaders.find(id) != s_shaders.end());

        OpenGLShader &shader = s_shaders[id];
        glDeleteProgram(shader.program);
        
        s_shaders.erase(id);
    }

    void OpenGLRenderDriver::CreateTexture(ResourceId id, const TextureDescriptor &descriptor) {
        HYP_ASSERT(s_textures.find(id) == s_textures.end());

        OpenGLTexture &texture = s_textures[id];
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

    void OpenGLRenderDriver::GetTextureData(ResourceId id, Vector<uint8> &data) {
        HYP_ASSERT(s_textures.find(id) != s_textures.end());

        OpenGLTexture &texture = s_textures[id];
        
        // FIXME: This is hardcoded for 2D textures.
        // Maybe store the complete size beforehand.
        GLsizei size = texture.size.height * texture.size.width * 4;
        data.resize(size);

        GLenum format = OpenGLUtilities::GetGLTextureFormat(texture.format);
        GLenum format_type = OpenGLUtilities::GetGLTextureFormatType(texture.format);

        glGetTextureImage(texture.texture, 0, format, format_type, size, data.data());
    }

    void OpenGLRenderDriver::DestroyTexture(ResourceId id) {
        HYP_ASSERT(s_textures.find(id) != s_textures.end());

        OpenGLTexture &texture = s_textures[id];
        glDeleteTextures(1, &texture.texture);

        s_textures.erase(id);
    }

    void OpenGLRenderDriver::CreateMaterial(ResourceId id, const MaterialDescriptor &descriptor) {
        HYP_ASSERT(s_materials.find(id) == s_materials.end());

        OpenGLMaterial &material = s_materials[id];
        HYP_ASSERT(s_shaders.find(descriptor.shader_id) != s_shaders.end());
        material.shader_id = descriptor.shader_id;

        CollectMaterialProperties(material);
    }

    void OpenGLRenderDriver::SetMaterialProperty(ResourceId id, const MaterialProperty &property) {
        HYP_ASSERT(s_materials.find(id) != s_materials.end());
        OpenGLMaterial &material = s_materials[id];
        
        ResourceId shader_id = material.shader_id;
        HYP_ASSERT(s_shaders.find(shader_id) != s_shaders.end());
        OpenGLShader &shader = s_shaders[shader_id];
        GLuint program = shader.program;

        MaterialPropertyId property_id = property.id;
        auto it = material.properties.find(property_id);
        if (it == material.properties.end()) {
            // We simply ignore a property if it does not actually exist on the shader.
            return;
        }
        // We still have to check that the types actually match.
        if (it->second.type != property.type) {
            HYP_LOG_ERROR("OpenGL", "Trying to assign material property '{}' a wrong type!", it->second.name);
            return;
        }

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
            case MaterialPropertyType::Texture: {
                ResourceId texture_id = property.storage.texture;
                HYP_ASSERT(s_textures.find(texture_id) != s_textures.end());
                OpenGLTexture &texture = s_textures[texture_id];

                GLuint texture_unit = static_cast<GLuint>(material.textures.size());
                HYP_ASSERT(texture_unit < static_cast<GLuint>(m_graphics_context->GetLimits().max_texture_units));
                glProgramUniform1i(program, location, texture_unit);
                material.textures[property_id] = texture.texture;
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
    }

    void OpenGLRenderDriver::DestroyMaterial(ResourceId id) {
        HYP_ASSERT(s_materials.find(id) != s_materials.end());
        s_materials.erase(id);
    }

    void OpenGLRenderDriver::CreateMesh(ResourceId id, const MeshDescriptor &descriptor) {
        HYP_ASSERT(s_meshes.find(id) == s_meshes.end());

        OpenGLMesh &mesh = s_meshes[id];
        mesh.index_format = descriptor.index_format;
        mesh.sub_meshes.assign(descriptor.sub_meshes.data, descriptor.sub_meshes.data + (descriptor.sub_meshes.size / sizeof(*descriptor.sub_meshes.data)));

        // TODO: Add ability to mark meshes dynamic
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

            GLuint attribute_index = i;
            GLuint binding_index = 0;
            GLint size = vertex_attribute.dimension;
            GLenum type = OpenGLUtilities::GetGLVertexAttributeType(vertex_attribute.type);

            glEnableVertexArrayAttrib(vertex_array_id, attribute_index);
            glVertexArrayAttribFormat(vertex_array_id, attribute_index, size, type, false, relative_offset);
            glVertexArrayVertexBuffer(vertex_array_id, binding_index, mesh.vertex_buffer, 0, stride);
            glVertexArrayAttribBinding(vertex_array_id, attribute_index, binding_index);

            relative_offset += OpenGLUtilities::GetGLSizeForVertexAttribute(vertex_attribute.type, vertex_attribute.dimension);
        }
    }

    void OpenGLRenderDriver::DrawMesh(ResourceId mesh_id, ResourceId material_id, uint32 sub_mesh_index) {
        HYP_ASSERT(s_materials.find(material_id) != s_materials.end());
        OpenGLMaterial &material = s_materials[material_id];
        UseMaterial(material);

        HYP_ASSERT(s_meshes.find(mesh_id) != s_meshes.end());
        OpenGLMesh &mesh = s_meshes[mesh_id];
        HYP_ASSERT(sub_mesh_index < mesh.sub_meshes.size());
        SubMesh &sub_mesh = mesh.sub_meshes[sub_mesh_index];

        GLenum index_format = OpenGLUtilities::GetGLIndexFormat(mesh.index_format);
        GLsizei index_format_size = OpenGLUtilities::GetGLIndexFormatSize(mesh.index_format);
        const void *offset = reinterpret_cast<const void *>(static_cast<uint64>(sub_mesh.index_offset) * index_format_size);
        glBindVertexArray(mesh.vertex_array);
        glDrawElementsBaseVertex(OpenGLUtilities::GetGLMeshTopology(sub_mesh.topology), sub_mesh.index_count, index_format, offset, sub_mesh.vertex_offset);
    }

    void OpenGLRenderDriver::DestroyMesh(ResourceId id) {
        HYP_ASSERT(s_meshes.find(id) != s_meshes.end());

        OpenGLMesh &mesh = s_meshes[id];
        glDeleteBuffers(2, &mesh.vertex_buffer);
        glDeleteVertexArrays(1, &mesh.vertex_array);

        s_meshes.erase(id);
    }

    void OpenGLRenderDriver::CreateTexture2D(OpenGLTexture &texture, const TextureDescriptor &descriptor) {
        glCreateTextures(GL_TEXTURE_2D, 1, &texture.texture);
        GLuint texture_id = texture.texture;

        TextureParameters parameters = descriptor.parameters;

        // Wrap mode
        {
            GLint wrap_mode = OpenGLUtilities::GetGLTextureWrapMode(parameters.wrap_mode);
            glTextureParameteri(texture_id, GL_TEXTURE_WRAP_S, wrap_mode);
            glTextureParameteri(texture_id, GL_TEXTURE_WRAP_T, wrap_mode);
        }

        // Filter
        {
            GLint min_filter = OpenGLUtilities::GetGLTextureMinFilter(parameters.filter);
            GLint mag_filter = OpenGLUtilities::GetGLTextureMaxFilter(parameters.filter);
            glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, min_filter);
            glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, mag_filter);
        }

        // Anisotropic filter
        {
            GLfloat anisotropic_filter_value = OpenGLUtilities::GetGLTextureAnisotropicFilter(parameters.anisotropic_filter);
            glTextureParameterf(texture_id, GL_TEXTURE_MAX_ANISOTROPY, anisotropic_filter_value);
        }

        TextureFormat format = descriptor.format;
        GLsizei width = descriptor.size.width;
        GLsizei height = descriptor.size.height;
        GLenum internal_format = OpenGLUtilities::GetGLTextureInternalFormat(format);
        glTextureStorage2D(texture_id, parameters.use_mipmaps ? Math::Max(descriptor.mipmap_count, 1) : 1, internal_format, width, height);

        if (descriptor.pixels.size > 0) {
            // RANT: Because OpenGL is retarded and the texture origin is in the bottom left corner,
            // we have to flip the image before uploading to the GPU.
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

    void OpenGLRenderDriver::CreateTextureCubemap(OpenGLTexture &texture, const TextureDescriptor &descriptor) {
        // TODO: Implement
    }

    void OpenGLRenderDriver::CollectMaterialProperties(OpenGLMaterial &material) {
        ResourceId shader_id = material.shader_id;
        HYP_ASSERT(s_shaders.find(shader_id) != s_shaders.end());
        OpenGLShader &shader = s_shaders[shader_id];
        GLuint program = shader.program;

        GLint active_uniform_count;
        glGetProgramInterfaceiv(program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &active_uniform_count);

        for (GLint uniform_index = 0; uniform_index < active_uniform_count; uniform_index++) {
            const uint64 NAME_BUFFER_COUNT = 50;
            String name_buffer;
            name_buffer.resize(NAME_BUFFER_COUNT);
            GLsizei name_length;
            glGetProgramResourceName(program, GL_UNIFORM, uniform_index, NAME_BUFFER_COUNT, &name_length, name_buffer.data());
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
                property.name = name_buffer;
                property.type = OpenGLUtilities::GetMaterialPropertyTypeForGLShaderType(type);
                property.location = location;
            }
        }
    }

    void OpenGLRenderDriver::UseMaterial(const OpenGLMaterial &material) {
        // TODO: We should keep track of the currently used material.
        // That way we can skip binding the shader and textures again.

        ResourceId shader_id = material.shader_id;
        HYP_ASSERT(s_shaders.find(shader_id) != s_shaders.end());
        OpenGLShader &shader = s_shaders[shader_id];
        glUseProgram(shader.program);

        GLuint texture_unit = 0;
        for (auto [property_id, texture] : material.textures) {
            glBindTextureUnit(texture_unit, texture);
            texture_unit++;
        }
    }

}