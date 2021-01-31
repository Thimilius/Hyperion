#include "hyppch.hpp"

#include "hyperion/driver/opengl/commands/opengl_command_buffer.hpp"

namespace Hyperion::Rendering {

    void OpenGLCommandBuffer::ClearCommands() {
        m_buffer.clear();
    }

    void OpenGLCommandBuffer::Clear(ClearFlags clear_flags, Color color) {
        OpenGLCommandClear *command = Allocate<OpenGLCommandClear>(OpenGLCommandType::Clear);
        command->clear_flags = clear_flags;
        command->color = color;
    }

    void OpenGLCommandBuffer::SetViewport(const Viewport &viewport) {
        OpenGLCommandSetViewport *command = Allocate<OpenGLCommandSetViewport>(OpenGLCommandType::SetViewport);
        command->viewport = viewport;
    }

    void OpenGLCommandBuffer::SetRasterizerState(const RasterizerState &rasterizer_state) {
        OpenGLCommandSetRasterizerState *command = Allocate<OpenGLCommandSetRasterizerState>(OpenGLCommandType::SetRasterizerState);
        command->rasterizer_state = rasterizer_state;
    }

    void OpenGLCommandBuffer::DrawMesh(ResourceId mesh_id, ResourceId material_id, uint32 sub_mesh_index) {
        OpenGLCommandDrawMesh *command = Allocate<OpenGLCommandDrawMesh>(OpenGLCommandType::DrawMesh);
        command->mesh_id = mesh_id;
        command->material_id = material_id;
        command->sub_mesh_index = sub_mesh_index;
    }

}