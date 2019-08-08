#pragma once

#include "hyperion/rendering/render_api.hpp"

namespace Hyperion::Rendering {

    class COpenGLRenderAPI : public CRenderAPI {
        void SetClearColor(float r, float g, float b, float a) override;
        void Clear() override;
        void SetViewport(s32 x, s32 y, s32 width, s32 height) override;
    };

}