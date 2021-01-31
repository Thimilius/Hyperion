#include "hyppch.hpp"

#include "hyperion/driver/opengl/commands/opengl_command_executor.hpp"

#include "hyperion/driver/opengl/opengl_render_driver.hpp"

namespace Hyperion::Rendering {

    uint64 OpenGLCommandExecutor::Execute(OpenGLRenderDriver *opengl_render_driver, OpenGLCommandType opengl_command_type, void *opengl_command) {
		switch (opengl_command_type) {
			case OpenGLCommandType::Clear: {
				auto command = reinterpret_cast<OpenGLCommandClear *>(opengl_command);
				opengl_render_driver->Clear(command->clear_flags, command->color);
				return sizeof(*command);
			}
			case OpenGLCommandType::SetViewport: {
				auto command = reinterpret_cast<OpenGLCommandSetViewport *>(opengl_command);
				opengl_render_driver->SetViewport(command->viewport);
				return sizeof(*command);
			}
			case OpenGLCommandType::DrawMesh: {
				auto command = reinterpret_cast<OpenGLCommandDrawMesh *>(opengl_command);
				opengl_render_driver->DrawMesh(command->mesh_id, command->material_id, command->sub_mesh_index);
				return sizeof(*command);
			}
			default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
		}
    }

}