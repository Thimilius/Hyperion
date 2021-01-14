#include "hyppch.hpp"

#include "hyperion/rendering/commands/render_command_executor.hpp"

#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    u64 RenderCommandExecutor::ExecuteRenderCommand(RenderCommandType command_type, const void *command, IRenderDriver *render_driver) {
        switch (command_type) {
            case RenderCommandType::Clear: {
                auto render_command = reinterpret_cast<const RenderCommandClear *>(command);
                render_driver->Clear(render_command->clear_flags, render_command->color);
                return sizeof(*render_command);
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}