#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class RenderThreadRenderDriver final : public IRenderDriver {
    public:
        RenderThreadRenderDriver(IRenderDriver *backend_render_driver);

        void Initialize(GraphicsContext *graphics_context) override { }
        void Shutdown() override { }

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
        void DestroyMesh(ResourceId mesh_id) override;

        CommandBuffer *CreateCommandBuffer() override;
        CommandBuffer *CopyCommandBuffer(CommandBuffer *command_buffer) override;
        void ExecuteCommandBuffer(CommandBuffer *command_buffer) override;
        void DestroyCommandBuffer(CommandBuffer *command_buffer) override;
    private:
        IRenderDriver *m_backend_render_driver;
    };

}