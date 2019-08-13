#pragma once

#include "render_api.hpp"
#include "vertex_array.hpp"

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

        inline static void SetClearColor(float r, float g, float b, float a) {
            s_render_api->SetClearColor(r, g, b, a);
        }

        inline static void Clear(EClearMask mask) {
            s_render_api->Clear(mask);
        }

        inline static void SetViewport(s32 x, s32 y, s32 width, s32 height) {
            s_render_api->SetViewport(x, y, width, height);
        }

        inline static void DrawIndexed(const TRef<CVertexArray> &vertex_array) {
            s_render_api->DrawIndexed(vertex_array);
        }
    private:
        static void Init();

        friend class CEngine;
    };

}