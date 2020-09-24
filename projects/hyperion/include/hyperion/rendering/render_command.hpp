#pragma once

#include "hyperion/core/app/display.hpp"
#include "hyperion/rendering/render_backend.hpp"
#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/render_texture.hpp"
#include "hyperion/rendering/buffer.hpp"

namespace Hyperion::Rendering {
    class RenderEngine;
}

namespace Hyperion::Rendering {

    class RenderCommand {
    public:
        inline static RasterizerState *GetRasterizerState() {
            return s_render_driver->GetRasterizerState();
        }

        inline static void Clear(ClearMask mask) {
            s_render_driver->Clear(mask);
        }

        inline static void Clear(ClearMask mask, Color color) {
            s_render_driver->Clear(mask, color);
        }

        inline static void SetViewport(u32 x, u32 y, u32 width, u32 height) {
            s_render_driver->SetViewport(x, y, width, height);
        }

        inline static void SetActiveRenderTarget(RenderTexture *texture, RenderTextureTarget target = RenderTextureTarget::DrawAndRead) {
            s_render_driver->SetActiveRenderTarget(texture, target);
            u32 width = texture ? texture->GetWidth() : Display::GetWidth();
            u32 height = texture ? texture->GetHeight() : Display::GetHeight();
            s_render_driver->SetViewport(0, 0, width, height);
        }

        inline static void Blit(RenderTexture *destination, RenderTexture *source) {
            u32 destination_width = destination ? destination->GetWidth() : Display::GetWidth();
            u32 destination_height = destination ? destination->GetHeight() : Display::GetHeight();
            s_render_driver->Blit(destination, 0, 0, destination_width, destination_height, source, 0, 0, source->GetWidth(), source->GetHeight());
        }

        inline static void Blit(RenderTexture *destination, u32 dstX0, u32 dstY0, u32 dstX1, u32 dstY1, RenderTexture *source, u32 srcX0, u32 srcY0, u32 srcX1, u32 srcY1) {
            s_render_driver->Blit(destination, dstX0, dstY0, dstX1, dstY1, source, srcX0, srcY0, srcX1, srcY1);
        }

        inline static void DrawIndexed(MeshTopology topology, IndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) {
            s_render_driver->DrawIndexed(topology, format, index_count, index_offset, vertex_offset);
        }

        inline static void Draw(MeshTopology topology, u32 vertex_count, u32 vertex_offset) {
            s_render_driver->Draw(topology, vertex_count, vertex_offset);
        }
    private:
        inline static RenderDriver *s_render_driver;
    private:
        friend class Hyperion::Rendering::RenderEngine;
    };

}