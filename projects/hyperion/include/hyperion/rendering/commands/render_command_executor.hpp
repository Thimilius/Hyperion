#pragma once

#include "hyperion/rendering/commands/render_commands.hpp"

namespace Hyperion::Rendering {

    class RenderCommandExecutor final {
    public:
        static uint64 ExecuteRenderCommand(RenderCommandType command_type, void *command, IRenderDriver *render_driver);
    private:
        RenderCommandExecutor() = delete;
        ~RenderCommandExecutor() = delete;
    };

}