#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_render_driver.hpp"

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"
#include "hyperion/driver/opengl/opengl_utilities.hpp"

namespace Hyperion::Rendering {

    void OpenGLRenderDriver::Clear(ClearFlags clear_flags, Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(OpenGLUtilities::GetGLClearFlags(clear_flags));
    }

    void OpenGLRenderDriver::Viewport(const Rendering::Viewport &viewport) {
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

        OpenGLShaderCompilationResult compilation_result = OpenGLShaderCompiler::Compile(descriptor.vertex, descriptor.fragment);
        if (compilation_result.succes) {
            OpenGLShader &shader = s_shaders[id];
            shader.program = compilation_result.program;
        } else {
            // TODO: How do we handle shader errors here?
        }
    }

    void OpenGLRenderDriver::FreeShader(ResourceId id) {
        HYP_ASSERT(s_shaders.find(id) != s_shaders.end());

        OpenGLShader &shader = s_shaders[id];
        glDeleteProgram(shader.program);
        
        s_shaders.erase(id);
    }

    void OpenGLRenderDriver::CreateMesh(ResourceId id, const MeshDescriptor &descriptor) {
        HYP_ASSERT(s_meshes.find(id) == s_meshes.end());

        OpenGLMesh &mesh = s_meshes[id];
        mesh.index_format = descriptor.index_format;
        mesh.sub_meshes = descriptor.sub_meshes;

        glCreateBuffers(2, &mesh.vertex_buffer);
        glNamedBufferData(mesh.vertex_buffer, descriptor.vertex_data.size(), descriptor.vertex_data.data(), GL_STATIC_DRAW);
        glNamedBufferData(mesh.index_buffer, descriptor.index_data.size(), descriptor.index_data.data(), GL_STATIC_DRAW);

        glCreateVertexArrays(1, &mesh.vertex_array);
        GLuint vertex_array_id = mesh.vertex_array;

        glVertexArrayElementBuffer(vertex_array_id, mesh.index_buffer);

        // FIXME: These are hardcoded attribute bindings
        glEnableVertexArrayAttrib(vertex_array_id, 0);
        glVertexArrayAttribFormat(vertex_array_id, 0, 3, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(vertex_array_id, 0, mesh.vertex_buffer, 0, 32);
        glVertexArrayAttribBinding(vertex_array_id, 0, 0);
    }

    void OpenGLRenderDriver::FreeMesh(ResourceId id) {
        HYP_ASSERT(s_meshes.find(id) != s_meshes.end());

        OpenGLMesh &mesh = s_meshes[id];
        glDeleteBuffers(2, &mesh.vertex_buffer);
        glDeleteVertexArrays(1, &mesh.vertex_array);

        s_meshes.erase(id);
    }

    void OpenGLRenderDriver::CreateTexture(ResourceId id, const TextureDescriptor &descriptor) {
        HYP_ASSERT(s_textures.find(id) == s_textures.end());

        OpenGLTexture &texture = s_textures[id];
        texture.dimension = descriptor.dimension;
        texture.format = descriptor.format;
        texture.parameters = descriptor.parameters;

        switch (descriptor.dimension) {
            case TextureDimension::Texture2D: CreateTexture2D(texture, descriptor); break;
            case TextureDimension::TextureCubemap: CreateTextureCubemap(texture, descriptor); break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

    void OpenGLRenderDriver::FreeTexture(ResourceId id) {
        HYP_ASSERT(s_meshes.find(id) != s_meshes.end());

        OpenGLTexture &texture = s_textures[id];
        glDeleteTextures(1, &texture.texture);

        s_textures.erase(id);
    }

    void OpenGLRenderDriver::DrawIndexed(ResourceId shader_id, ResourceId mesh_id) {
        HYP_ASSERT(s_shaders.find(shader_id) != s_shaders.end());
        HYP_ASSERT(s_meshes.find(mesh_id) != s_meshes.end());

        OpenGLShader &shader = s_shaders[shader_id];
        OpenGLMesh &mesh = s_meshes[mesh_id];

        glUseProgram(shader.program);
        glBindVertexArray(mesh.vertex_array);

        GLenum index_format = OpenGLUtilities::GetGLIndexFormat(mesh.index_format);
        GLsizei index_format_size = OpenGLUtilities::GetGLIndexFormatSize(mesh.index_format);
        for (SubMesh &sub_mesh : mesh.sub_meshes) {
            const void *offset = reinterpret_cast<const void *>(static_cast<uint64>(sub_mesh.index_offset) * index_format_size);
            glDrawElementsBaseVertex(OpenGLUtilities::GetGLMeshTopology(sub_mesh.topology), sub_mesh.index_count, index_format, offset, sub_mesh.vertex_offset);
        }
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
        glTextureStorage2D(texture_id, parameters.use_mipmaps ? descriptor.mipmap_count : 1, internal_format, width, height);

        if (descriptor.pixels.size() > 0) {
            OpenGLUtilities::SetUnpackAlignmentForTextureFormat(descriptor.format);

            GLenum format_value = OpenGLUtilities::GetGLTextureFormat(format);
            GLenum format_type = OpenGLUtilities::GetGLTextureFormatType(format);
            glTextureSubImage2D(texture_id, 0, 0, 0, width, height, format_value, format_type, descriptor.pixels.data());

            if (parameters.use_mipmaps) {
                glGenerateTextureMipmap(texture_id);
            }
        }
    }

    void OpenGLRenderDriver::CreateTextureCubemap(OpenGLTexture &texture, const TextureDescriptor &descriptor) {
        // TODO: Implement
    }

}