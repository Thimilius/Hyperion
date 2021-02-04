#pragma once

#include "hyperion/rendering/threading/render_thread_commands.hpp"

namespace Hyperion::Rendering {

    class RenderThreadCommandExecutor final {
    public:
        static uint64 Execute(IRenderDriver *render_driver, RenderThreadCommandType command_type, void *render_thread_command);
        static uint64 ExecuteImmediate(IRenderDriver *render_driver, ImmediateRenderThreadCommandType command_type, void *immediate_render_thread_command);
    private:
        RenderThreadCommandExecutor() = delete;
        ~RenderThreadCommandExecutor() = delete;
    };

}