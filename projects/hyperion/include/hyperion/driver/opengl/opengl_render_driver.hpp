#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class OpenGLRenderDriver : public RenderDriver {
    public:
        OpenGLRenderDriver();

        void Clear(ClearMask mask) override;
        void Clear(ClearMask mask, Color m_color) override;

        void SetViewport(u32 x, u32 y, u32 width, u32 height) override;
        
        void SetActiveRenderTarget(RenderTexture *texture, RenderTextureTarget target) override;
        void Blit(RenderTexture *destination, u32 dstX0, u32 dstY0, u32 dstX1, u32 dstY1, RenderTexture *source, u32 srcX0, u32 srcY0, u32 srcX1, u32 srcY1) override;

        void DrawIndexed(MeshTopology topology, IndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) override;
        void Draw(MeshTopology topology, u32 vertex_count, u32 vertex_offset) override;
    private:
        static u32 GetGLClearMask(ClearMask clear_mask);
        static u32 GetGLMeshTopology(MeshTopology topology);
        static u32 GetGLIndexFormat(IndexFormat index_format);
        static u64 GetGLIndexSize(IndexFormat index_format);
    };

}