#pragma once

#include "hyperion/rendering/threading/render_thread_commands.hpp"

namespace Hyperion::Rendering {

    class RenderThreadCommandExecutor final {
    public:
        static uint64 Execute(IRenderDriver *render_driver, RenderThreadCommandType command_type, void *render_thread_command);
        static void ExecuteImmediate(IRenderDriver *render_driver, ImmediateRenderThreadCommand &immediate_render_thread_command);
    private:
        RenderThreadCommandExecutor() = delete;
        ~RenderThreadCommandExecutor() = delete;
    };

}