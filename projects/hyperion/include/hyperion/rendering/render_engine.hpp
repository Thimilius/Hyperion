#pragma once

#include "hyperion/rendering/render_backend.hpp"
#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/render_texture.hpp"
#include "hyperion/rendering/buffer.hpp"
#include "hyperion/core/app/application.hpp"

namespace Hyperion::Rendering {

    class RenderEngine {
    private: 
        inline static RenderBackend s_render_backend;

        inline static Scope<RenderDriver> s_render_driver;
    public:
        inline static void EnableFeature(RenderFeature feature) {
            s_render_driver->EnableFeature(feature);
        }

        inline static void DisableFeature(RenderFeature feature) {
            s_render_driver->DisableFeature(feature);
        }

        inline static void SetFrontFaceMode(FrontFaceMode front_face_mode) {
            s_render_driver->SetFrontFaceMode(front_face_mode);
        }

        inline static void SetCullingMode(CullingMode culling_mode) {
            s_render_driver->SetCullingMode(culling_mode);
        }

        inline static void SetBlendFunc(BlendFactor source_factor, BlendFactor destination_factor) {
            s_render_driver->SetBlendFunc(source_factor, destination_factor);
        }

        inline static void SetBlendEquation(BlendEquation blend_equation) {
            s_render_driver->SetBlendEquation(blend_equation);
        }

        inline static void SetPolygonMode(PolygonMode polygon_mode) {
            s_render_driver->SetPolygonMode(polygon_mode);
        }

        inline static void SetClearColor(f32 r, f32 g, f32 b, f32 a) {
            s_render_driver->SetClearColor(r, g, b, a);
        }

        inline static void Clear(ClearMask mask) {
            s_render_driver->Clear(mask);
        }

        inline static void SetViewport(s32 x, s32 y, s32 width, s32 height) {
            s_render_driver->SetViewport(x, y, width, height);
        }

        inline static void SetActiveRenderTarget(const Ref<RenderTexture> &texture, RenderTextureTarget target = RenderTextureTarget::DrawAndRead) {
            s_render_driver->SetActiveRenderTarget(texture, target);
        }

        inline static void Blit(const Ref<RenderTexture> &destination, const Ref<RenderTexture> &source) {
            u32 destination_width = destination ? destination->GetWidth() : Application::GetInstance()->GetWindow()->GetWidth();
            u32 destination_height = destination ? destination->GetHeight() : Application::GetInstance()->GetWindow()->GetHeight();
            s_render_driver->Blit(destination, 0, 0, destination_width, destination_height, source, 0, 0, source->GetWidth(), source->GetHeight());
        }

        inline static void Blit(const Ref<RenderTexture> &destination, s32 dstX0, s32 dstY0, s32 dstX1, s32 dstY1, const Ref<RenderTexture> &source, s32 srcX0, s32 srcY0, s32 srcX1, s32 srcY1) {
            s_render_driver->Blit(destination, dstX0, dstY0, dstX1, dstY1, source, srcX0, srcY0, srcX1, srcY1);
        }

        inline static void DrawIndexed(Primitive primitive, IndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) {
            s_render_driver->DrawIndexed(primitive, format, index_count, index_offset, vertex_offset);
        }

        inline static void Draw(Primitive primitive, u32 vertex_count, u32 vertex_offset) {
            s_render_driver->Draw(primitive, vertex_count, vertex_offset);
        }

        inline static RenderBackend GetBackend() { return s_render_backend; }
    private:
        RenderEngine() = delete;
        ~RenderEngine() = delete;

        static void Init(RenderBackend backend);
        static void Shutdown();

        friend class Hyperion::Engine;
    };

}