#include "hyppch.hpp"

#include "hyperion/rendering/multithreaded_render_driver.hpp"

#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    void MultithreadedRenderDriver::Clear(ClearMask clear_mask, Color color) {
        RenderCommandClear *clear = RenderEngine::GetCommandQueue().Allocate<RenderCommandClear>(RenderCommandType::Clear);
        clear->clear_mask = clear_mask;
        clear->color = color;
    }

}