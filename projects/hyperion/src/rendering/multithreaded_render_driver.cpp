#include "hyppch.hpp"

#include "hyperion/rendering/multithreaded_render_driver.hpp"

#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    void MultithreadedRenderDriver::Clear(ClearFlags clear_flags, Color color) {
        RenderCommandClear *clear = RenderEngine::GetCommandQueue().Allocate<RenderCommandClear>(RenderCommandType::Clear);
        clear->clear_flags = clear_flags;
        clear->color = color;
    }

}