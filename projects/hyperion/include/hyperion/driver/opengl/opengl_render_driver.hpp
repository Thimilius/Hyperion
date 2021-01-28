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
        void DestroyShader(ResourceId id) override;

        void CreateMesh(ResourceId id, const MeshDescriptor &descriptor) override;
        void DestroyMesh(ResourceId id) override;

        void CreateTexture(ResourceId id, const TextureDescriptor &descriptor) override;
        void GetTextureData(ResourceId id, Vector<uint8> &data) override;
        void DestroyTexture(ResourceId id) override;

        void CreateMaterial(ResourceId id, const MaterialDescriptor &descriptor) override;
        void SetMaterialProperty(ResourceId id, const MaterialProperty &property) override;
        void DestroyMaterial(ResourceId id) override;

        void DrawIndexed(ResourceId shader_id, ResourceId mesh_id) override;
    private:
        void CreateTexture2D(OpenGLTexture &texture, const TextureDescriptor &descriptor);
        void CreateTextureCubemap(OpenGLTexture &texture, const TextureDescriptor &descriptor);

        static void FlipTextureHorizontally(uint8 *data, uint32 texture_height, uint32 texture_stride);
        static uint32 GetBytesPerPixelForTextureFormat(TextureFormat format);
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
            TextureSize size;
        };
        inline static Map<ResourceId, OpenGLTexture> s_textures;

        struct OpenGLMaterial {
            ResourceId shader;
        };
        inline static Map<ResourceId, OpenGLMaterial> s_materials;
    };

}