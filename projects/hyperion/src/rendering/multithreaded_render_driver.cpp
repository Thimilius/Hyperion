#include "hyppch.hpp"

#include "hyperion/rendering/multithreaded_render_driver.hpp"

#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    void MultithreadedRenderDriver::Clear(ClearMask clear_mask, Color color) {
        RenderCommand command = { };
        command.type = RenderCommandType::Clear;
        command.clear.clear_mask = clear_mask;
        command.clear.color = color;
        RenderEngine::s_update_queue.push(command);
    }

}