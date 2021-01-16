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
            case RenderCommandType::Viewport: {
                auto render_command = reinterpret_cast<const RenderCommandViewport *>(command);
                render_driver->Viewport(render_command->viewport);
                return sizeof(*render_command);
            }
            case RenderCommandType::CreateShader: {
                auto render_command = reinterpret_cast<const RenderCommandCreateShader *>(command);
                render_driver->CreateShader(render_command->id, render_command->descriptor);
                return sizeof(*render_command);
            }
            case RenderCommandType::FreeShader: {
                auto render_command = reinterpret_cast<const RenderCommandId *>(command);
                render_driver->FreeShader(render_command->id);
                return sizeof(*render_command);
            }
            case RenderCommandType::CreateMesh: {
                auto render_command = reinterpret_cast<const RenderCommandCreateMesh *>(command);
                render_driver->CreateMesh(render_command->id, render_command->descriptor);
                return sizeof(*render_command);
            }
            case RenderCommandType::FreeMesh: {
                auto render_command = reinterpret_cast<const RenderCommandId *>(command);
                render_driver->FreeMesh(render_command->id);
                return sizeof(*render_command);
            }
            case RenderCommandType::DrawIndexed: {
                auto render_command = reinterpret_cast<const RenderCommandDrawIndexed *>(command);
                render_driver->DrawIndexed(render_command->shader_id, render_command->mesh_id);
                return sizeof(*render_command);
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}