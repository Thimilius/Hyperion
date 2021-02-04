#pragma once

#include "hyperion/rendering/threading/render_thread_commands.hpp"

namespace Hyperion::Rendering {

    class RenderThreadCommandExecutor final {
    public:
        static uint64 Execute(IRenderDriver *render_driver, RenderThreadCommandType command_type, void *render_thread_command);
        static uint64 ExecuteQuery(IRenderDriver *render_driver, RenderThreadQueryCommandType command_type, void *render_thread_query_command);
    private:
        RenderThreadCommandExecutor() = delete;
        ~RenderThreadCommandExecutor() = delete;
    };

}