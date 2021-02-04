#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class RenderThreadRenderDriver final : public IRenderDriver {
    public:
        void Initialize(GraphicsContext *graphics_context) override { }
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
    };

}