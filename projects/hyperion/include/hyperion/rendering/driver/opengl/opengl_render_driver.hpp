#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/driver/render_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class OpenGLRenderDriver final : public IRenderDriver {
    public:
        void Initialize() override;
        void Render(RenderFrame *render_frame) override;
        void Shutdown() override;
    };

}