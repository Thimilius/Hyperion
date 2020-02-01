#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class OpenGLRenderDriver : public RenderDriver {
    public:
        OpenGLRenderDriver();

        String GetShaderExtension() const override { return ".glsl"; }

        void Clear(ClearMask mask) override;
        void Clear(ClearMask mask, Color color) override;

        void SetViewport(s32 x, s32 y, s32 width, s32 height) override;
        
        void SetActiveRenderTarget(const Ref<RenderTexture> &texture, RenderTextureTarget target) override;
        void Blit(const Ref<RenderTexture> &destination, s32 dstX0, s32 dstY0, s32 dstX1, s32 dstY1, const Ref<RenderTexture> &source, s32 srcX0, s32 srcY0, s32 srcX1, s32 srcY1) override;

        void DrawIndexed(PrimitiveType primitive, IndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) override;
        void Draw(PrimitiveType primitive, u32 vertex_count, u32 vertex_offset) override;
    private:
        static u32 GetGLClearMask(ClearMask clear_mask);
        static u32 GetGLPrimitiveType(PrimitiveType primitive);
        static u32 GetGLIndexFormat(IndexFormat index_format);
        static u64 GetGLIndexSize(IndexFormat index_format);
    };

}