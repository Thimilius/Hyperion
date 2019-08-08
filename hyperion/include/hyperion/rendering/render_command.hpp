#pragma once

#include "render_api.hpp"

namespace Hyperion::Rendering {

    class CRenderCommand {
    private: 
        inline static CRenderAPI *s_render_api;
    public:
        inline static void SetClearColor(float r, float g, float b, float a) {
            s_render_api->SetClearColor(r, g, b, a);
        }

        inline static void Clear() {
            s_render_api->Clear();
        }

        inline static void SetViewport(s32 x, s32 y, s32 width, s32 height) {
            s_render_api->SetViewport(x, y, width, height);
        }
    private:
        static void Init();

        friend class CEngine;
    };

}