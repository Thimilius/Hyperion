#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/frame/context/render_frame_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderFrame final : public INonCopyable {
    public:
        void Clear();

        RenderFrameContext &GetContext() { return m_context; }
        const RenderFrameContext &GetContext() const { return m_context; }
    private:
        RenderFrameContext m_context;
    };

}