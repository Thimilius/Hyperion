#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class RenderTexture;

    namespace Rendering {
        class RenderFrame;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class RenderPipeline {
        Forward,
        Deferred,

        Custom
    };

    class IRenderPipeline {
    public:
        virtual ~IRenderPipeline() = default;
    public:
        virtual void Initialize() = 0;
        virtual void Render(RenderFrame *render_frame) = 0;
        virtual void Shutdown() = 0;

        virtual uint32 GetRenderTargetWidth() const = 0;
        virtual uint32 GetRenderTargetHeight() const = 0;
        virtual void SetRenderTargetSize(uint32 width, uint32 height) = 0;
        virtual RenderTexture *GetTargetRenderTexture() const = 0;

        virtual void SetShouldBlitToScreen(bool8 should_blit_to_screen) = 0;
    };

}