#pragma once

#include "render_api.hpp"
#include "render_texture.hpp"
#include "buffer.hpp"

namespace Hyperion::Rendering {

    class CRenderCommand {
    private: 
        inline static CRenderAPI *s_render_api;
    public:
        inline static void EnableFeature(EFeature feature) {
            s_render_api->EnableFeature(feature);
        }

        inline static void DisableFeature(EFeature feature) {
            s_render_api->DisableFeature(feature);
        }

        inline static void SetFrontFaceMode(EFrontFaceMode front_face_mode) {
            s_render_api->SetFrontFaceMode(front_face_mode);
        }

        inline static void SetCullingMode(ECullingMode culling_mode) {
            s_render_api->SetCullingMode(culling_mode);
        }

        inline static void SetBlendFunc(EBlendFactor source_factor, EBlendFactor destination_factor) {
            s_render_api->SetBlendFunc(source_factor, destination_factor);
        }

        inline static void SetBlendEquation(EBlendEquation blend_equation) {
            s_render_api->SetBlendEquation(blend_equation);
        }

        inline static void SetPolygonMode(EPolygonMode polygon_mode) {
            s_render_api->SetPolygonMode(polygon_mode);
        }

        inline static void SetClearColor(float r, float g, float b, float a) {
            s_render_api->SetClearColor(r, g, b, a);
        }

        inline static void Clear(EClearMask mask) {
            s_render_api->Clear(mask);
        }

        inline static void SetViewport(s32 x, s32 y, s32 width, s32 height) {
            s_render_api->SetViewport(x, y, width, height);
        }

        inline static void SetActiveRenderTarget(const TRef<CRenderTexture> texture, ERenderTextureTarget target = ERenderTextureTarget::DrawAndRead) {
            s_render_api->SetActiveRenderTarget(texture, target);
        }

        inline static void Blit(const TRef<CRenderTexture> destination, s32 dstX0, s32 dstY0, s32 dstX1, s32 dstY1, const TRef<CRenderTexture> source, s32 srcX0, s32 srcY0, s32 srcX1, s32 srcY1) {
            s_render_api->Blit(destination, dstX0, dstY0, dstX1, dstY1, source, srcX0, srcY0, srcX1, srcY1);
        }

        inline static void DrawIndexed(EIndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) {
            s_render_api->DrawIndexed(format, index_count, index_offset, vertex_offset); 
        }

        inline static void Draw(u32 vertex_count, u32 vertex_offset) {
            s_render_api->Draw(vertex_count, vertex_offset);
        }
    private:
        static void Init();

        friend class CEngine;
    };

}