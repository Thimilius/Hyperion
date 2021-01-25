#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class MultithreadedRenderDriver final : public IRenderDriver {
    public:
        void Clear(ClearFlags clear_flags, Color color) override;
        void Viewport(const Rendering::Viewport &viewport) override;
        void SetRasterizerState(const RasterizerState &rasterizer_state) override;

        void CreateShader(ResourceId id, const ShaderDescriptor &descriptor) override;
        void DestroyShader(ResourceId id) override;

        void CreateMesh(ResourceId id, const MeshDescriptor &descriptor) override;
        void DestroyMesh(ResourceId id) override;

        void CreateTexture(ResourceId id, const TextureDescriptor &descriptor) override;
        void DestroyTexture(ResourceId id) override;

        void CreateMaterial(ResourceId id, const MaterialDescriptor &descriptor) override;
        void SetMaterialProperty(ResourceId id, const MaterialProperty &property) override;
        void DestroyMaterial(ResourceId id) override;

        void DrawIndexed(ResourceId mesh_id, ResourceId material_id) override;
    };

}