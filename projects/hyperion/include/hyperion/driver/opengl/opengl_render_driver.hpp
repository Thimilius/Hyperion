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
        void SetCameraData(const CameraData &camera_data) override;

        void CreateShader(ResourceId shader_id, const ShaderDescriptor &descriptor) override;
        void DestroyShader(ResourceId shader_id) override;

        void CreateTexture(ResourceId texture_id, const TextureDescriptor &descriptor) override;
        void GetTextureData(ResourceId texture_id, Vector<uint8> *buffer, GetTextureDataCallback callback) override;
        void DestroyTexture(ResourceId texture_id) override;

        void CreateMaterial(ResourceId material_id, const MaterialDescriptor &descriptor) override;
        void SetMaterialProperty(ResourceId material_id, const MaterialProperty &property) override;
        void DestroyMaterial(ResourceId material_id) override;

        void CreateRenderTexture(ResourceId render_texture_id, const RenderTextureDescriptor &descriptor) override;
        void ResizeRenderTexture(ResourceId render_texture_id, uint32 width, uint32 height, uint32 mipmap_count) override;
        void SetRenderTexture(ResourceId render_texture_id) override;
        void BlitRenderTexture(ResourceId destination_id, uint32 destination_width, uint32 destination_height, ResourceId source_id, uint32 source_width, uint32 source_height) override;
        void DestroyRenderTexture(ResourceId render_texture_id) override;

        void CreateMesh(ResourceId mesh_id, const MeshDescriptor &descriptor) override;
        void DrawMesh(ResourceId mesh_id, const Mat4 &model_matrix, ResourceId material_id, uint32 sub_mesh_index) override;
        void DestroyMesh(ResourceId mesh_id) override;
    private:
        void CreateTexture2D(OpenGLTexture &texture, const TextureDescriptor &descriptor);
        void CreateTextureCubemap(OpenGLTexture &texture, const TextureDescriptor &descriptor);
        void SetTextureParameters(GLuint texture, const TextureParameters &parameters);

        void CollectMaterialProperties(OpenGLMaterial &material);
        void UseMaterial(OpenGLMaterial &material, const Mat4 &model_matrix);
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

        enum class OpenGLRenderTextureAttachmentType {
            Texture,
            Renderbuffer
        };
        struct OpenGLRenderTextureAttachment {
            GLuint attachment;

            RenderTextureAttachment attributes;
        };
        struct OpenGLRenderTexture {
            GLuint render_texture;

            TextureSize size;
            uint32 mipmap_count;

            uint32 color_attachment_count;
            Vector<OpenGLRenderTextureAttachment> attachments;
        };
        Map<ResourceId, OpenGLRenderTexture> m_render_textures;

        struct OpenGLMesh {
            // Vertex and index buffer have to be next to each other to support efficient creation and destruction.
            GLuint vertex_buffer;
            GLuint index_buffer;
            GLuint vertex_array;

            IndexFormat index_format;
            Vector<SubMesh> sub_meshes;
        };
        Map<ResourceId, OpenGLMesh> m_meshes;

        // We want to keep track of the state of a few resources.
        CameraData m_current_camera_data;
        const OpenGLMaterial *m_current_material = nullptr;
        const OpenGLRenderTexture *m_current_render_texture = nullptr;
    };

}