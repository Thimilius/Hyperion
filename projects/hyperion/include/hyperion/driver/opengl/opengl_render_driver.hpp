#pragma once

#include <glad/glad.h>

#include "hyperion/driver/opengl/opengl_graphics_context.hpp"
#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class OpenGLRenderDriver : public IRenderDriver {
        struct OpenGLTexture;
        struct OpenGLMaterial;
    public:
        void Initialize(GraphicsContext *graphics_context) override;
        void Shutdown() override { }

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

        void CreateRenderTexture(ResourceId render_texture_id, const RenderTextureDescriptor &descriptor) override;
        void DestroyRenderTexture(ResourceId render_texture_id) override;

        void CreateMesh(ResourceId mesh_id, const MeshDescriptor &descriptor) override;
        void DrawMesh(ResourceId mesh_id, ResourceId material_id, uint32 sub_mesh_index) override;
        void DestroyMesh(ResourceId mesh_id) override;
    private:
        void CreateTexture2D(OpenGLTexture &texture, const TextureDescriptor &descriptor);
        void CreateTextureCubemap(OpenGLTexture &texture, const TextureDescriptor &descriptor);
        void SetTextureParameters(GLuint texture, const TextureParameters &parameters);

        void CollectMaterialProperties(OpenGLMaterial &material);
        void UseMaterial(const OpenGLMaterial &material);
    private:
        OpenGLGraphicsContext *m_graphics_context;

        struct OpenGLShader {
            GLuint program;
        };
        Map<ResourceId, OpenGLShader> m_shaders;
        OpenGLShader m_fallback_shader;

        struct OpenGLTexture {
            GLuint texture;

            TextureDimension dimension;
            TextureFormat format;
            TextureParameters parameters;
            TextureSize size;
        };
        Map<ResourceId, OpenGLTexture> m_textures;

        struct OpenGLMaterialProperty {
            MaterialPropertyType type;
            String name;

            GLint location;
        };
        struct OpenGLMaterial {
            ResourceId shader_id;

            Map<MaterialPropertyId, OpenGLMaterialProperty> properties;
            Map<MaterialPropertyId, GLuint> textures;
        };
        Map<ResourceId, OpenGLMaterial> m_materials;
        const OpenGLMaterial *m_current_material = nullptr;

        enum class OpenGLRenderTextureAttachmentType {
            Texture,
            Renderbuffer
        };
        struct OpenGLRenderTextureAttachment {
            OpenGLRenderTextureAttachmentType type;
            GLuint attachment;
        };
        struct OpenGLRenderTexture {
            GLuint render_texture;

            Vector<OpenGLRenderTextureAttachment> attachments;
        };
        Map<ResourceId, OpenGLRenderTexture> m_render_textures;

        struct OpenGLMesh {
            // Vertex and index buffer have to be next to each other to support efficient creation and destruction
            GLuint vertex_buffer;
            GLuint index_buffer;
            GLuint vertex_array;

            IndexFormat index_format;
            Vector<SubMesh> sub_meshes;
        };
        Map<ResourceId, OpenGLMesh> m_meshes;
    };

}