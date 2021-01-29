#pragma once

#include <glad/glad.h>

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class OpenGLRenderDriver : public IRenderDriver {
        struct OpenGLTexture;
        struct OpenGLMaterial;
    public:
        void Clear(ClearFlags clear_flags, Color color) override;
        void SetViewport(const Viewport &viewport) override;
        void SetRasterizerState(const RasterizerState &rasterizer_state) override;

        void CreateShader(ResourceId shader_id, const ShaderDescriptor &descriptor) override;
        void DestroyShader(ResourceId shader_id) override;

        void CreateTexture(ResourceId texture_id, const TextureDescriptor &descriptor) override;
        void GetTextureData(ResourceId texture_id, Vector<uint8> &data) override;
        void DestroyTexture(ResourceId texture_id) override;

        void CreateMaterial(ResourceId material_id, const MaterialDescriptor &descriptor) override;
        void SetMaterialProperty(ResourceId material_id, const MaterialProperty &property) override;
        void DestroyMaterial(ResourceId material_id) override;

        void CreateMesh(ResourceId mesh_id, const MeshDescriptor &descriptor) override;
        void DrawMesh(ResourceId mesh_id, ResourceId material_id, uint32 sub_mesh_index) override;
        void DestroyMesh(ResourceId mesh_id) override;
    private:
        static void CreateTexture2D(OpenGLTexture &texture, const TextureDescriptor &descriptor);
        static void CreateTextureCubemap(OpenGLTexture &texture, const TextureDescriptor &descriptor);

        static void CollectMaterialProperties(OpenGLMaterial &material);

        static void UseMaterial(const OpenGLMaterial &material);
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
            ResourceId shader_id;

            Map<MaterialPropertyId, GLint> locations;
            Map<MaterialPropertyId, GLuint> textures;
        };
        inline static Map<ResourceId, OpenGLMaterial> s_materials;
    };

}