#pragma once

#include "hyperion/rendering/render_api.hpp"

namespace Hyperion::Rendering {

    class OpenGLRenderAPI : public RenderAPI {
    public:
        OpenGLRenderAPI();

        void EnableFeature(Feature feature) override;
        void DisableFeature(Feature feature) override;

        void SetBlendFunc(BlendFactor source_factor, BlendFactor destination_factor) override;
        void SetBlendEquation(BlendEquation blend_equation) override;

        void SetFrontFaceMode(FrontFaceMode front_face_mode) override;
        void SetCullingMode(CullingMode culling_mode) override;

        void SetPolygonMode(PolygonMode polygon_mode) override;

        void SetClearColor(f32 r, f32 g, f32 b, f32 a) override;
        void Clear(ClearMask mask) override;

        void SetViewport(s32 x, s32 y, s32 width, s32 height) override;
        
        void SetActiveRenderTarget(const Ref<RenderTexture> &texture, RenderTextureTarget target) override;
        void Blit(const Ref<RenderTexture> &destination, s32 dstX0, s32 dstY0, s32 dstX1, s32 dstY1, const Ref<RenderTexture> &source, s32 srcX0, s32 srcY0, s32 srcX1, s32 srcY1) override;

        void DrawIndexed(Primitive primitive, IndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) override;
        void Draw(Primitive primitive, u32 vertex_count, u32 vertex_offset) override;
    private:
        static u32 GetGLFeature(Feature feature);
        static u32 GetGLBlendFactor(BlendFactor blend_factor);
        static u32 GetGLBlendEquation(BlendEquation blend_equation);
        static u32 GetGLFrontFaceMode(FrontFaceMode front_face_mode);
        static u32 GetGLPolygonMode(PolygonMode polygon_mode);
        static u32 GetGLCullingMode(CullingMode culling_mode);
        static u32 GetGLClearMask(ClearMask clear_mask);
        static u32 GetGLPrimitive(Primitive primitive);
        static u32 GetGLIndexFormat(IndexFormat index_format);
        static u64 GetGLIndexSize(IndexFormat index_format);
    };

}