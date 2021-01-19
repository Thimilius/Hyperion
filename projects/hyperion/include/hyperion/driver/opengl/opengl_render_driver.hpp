#pragma once

#include <glad/glad.h>

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class OpenGLRenderDriver : public IRenderDriver {
        struct OpenGLTexture;
    public:
        void Clear(ClearFlags clear_flags, Color color) override;
        void Viewport(const Rendering::Viewport &viewport) override;
        void SetRasterizerState(const RasterizerState &rasterizer_state) override;

        void CreateShader(ResourceId id, const ShaderDescriptor &descriptor) override;
        void FreeShader(ResourceId id) override;

        void CreateMesh(ResourceId id, const MeshDescriptor &descriptor) override;
        void FreeMesh(ResourceId id) override;

        void CreateTexture(ResourceId id, const TextureDescriptor &descriptor) override;
        void FreeTexture(ResourceId id) override;

        void DrawIndexed(ResourceId shader_id, ResourceId mesh_id) override;
    private:
        void CreateTexture2D(OpenGLTexture &texture, const TextureDescriptor &descriptor);
        void CreateTextureCubemap(OpenGLTexture &texture, const TextureDescriptor &descriptor);
    private:
        struct OpenGLShader {
            GLuint program;
        };
        inline static Map<ResourceId, OpenGLShader> s_shaders;

        struct OpenGLMesh {
            // Vertex and index buffer have to be next to each other to support efficient creation and destruction
            GLuint vertex_buffer;
            GLuint index_buffer;
            GLuint vertex_array;

            IndexFormat index_format;
            Vector<SubMesh> sub_meshes;
        };
        inline static Map<ResourceId, OpenGLMesh> s_meshes;

        struct OpenGLTexture {
            GLuint texture;

            TextureDimension dimension;
            TextureFormat format;
            TextureParameters parameters;
        };
        inline static Map<ResourceId, OpenGLTexture> s_textures;
    };

}