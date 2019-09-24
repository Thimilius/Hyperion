#pragma once

#include "hyperion/rendering/render_api.hpp"

namespace Hyperion::Rendering {

    class COpenGLRenderAPI : public CRenderAPI {
    public:
        void EnableFeature(EFeature feature) override;
        void DisableFeature(EFeature feature) override;

        void SetBlendFunc(EBlendFactor source_factor, EBlendFactor destination_factor) override;
        void SetBlendEquation(EBlendEquation blend_equation) override;

        void SetFrontFaceMode(EFrontFaceMode front_face_mode) override;
        void SetCullingMode(ECullingMode culling_mode) override;

        void SetPolygonMode(EPolygonMode polygon_mode) override;

        void SetClearColor(float r, float g, float b, float a) override;
        void Clear(EClearMask mask) override;

        void SetViewport(s32 x, s32 y, s32 width, s32 height) override;
        
        void SetActiveRenderTarget(const TRef<CRenderTexture> texture, ERenderTextureTarget target) override;
        void Blit(const TRef<CRenderTexture> destination, s32 dstX0, s32 dstY0, s32 dstX1, s32 dstY1, const TRef<CRenderTexture> source, s32 srcX0, s32 srcY0, s32 srcX1, s32 srcY1) override;

        void DrawIndexed(const TRef<CIndexBuffer> &index_buffer) override;
    private:
        static u32 GetGLFeature(EFeature feature);
        static u32 GetGLBlendFactor(EBlendFactor blend_factor);
        static u32 GetGLBlendEquation(EBlendEquation blend_equation);
        static u32 GetGLFrontFaceMode(EFrontFaceMode front_face_mode);
        static u32 GetGLPolygonMode(EPolygonMode polygon_mode);
        static u32 GetGLCullingMode(ECullingMode culling_mode);
        static u32 GetGLClearMask(EClearMask clear_mask);
        static u32 GetGLIndexFormat(EIndexFormat index_format);
    };

}