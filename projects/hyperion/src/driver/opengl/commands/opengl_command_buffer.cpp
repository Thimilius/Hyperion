#include "hyppch.hpp"

#include "hyperion/driver/opengl/commands/opengl_command_buffer.hpp"

namespace Hyperion::Rendering {

    void OpenGLCommandBuffer::ClearCommands() {
        m_buffer.clear();
    }

    void OpenGLCommandBuffer::Clear(ClearFlags clear_flags, Color color) {
        auto *command = Allocate<OpenGLCommandClear>(OpenGLCommandType::Clear);
        command->clear_flags = clear_flags;
        command->color = color;
    }

    void OpenGLCommandBuffer::SetViewport(const Viewport &viewport) {
        auto *command = Allocate<OpenGLCommandSetViewport>(OpenGLCommandType::SetViewport);
        command->viewport = viewport;
    }

    void OpenGLCommandBuffer::SetRasterizerState(const RasterizerState &rasterizer_state) {
        auto *command = Allocate<OpenGLCommandSetRasterizerState>(OpenGLCommandType::SetRasterizerState);
        command->rasterizer_state = rasterizer_state;
    }

    void OpenGLCommandBuffer::SetRenderTexture(RenderTexture *render_texture) {
        HYP_ASSERT(render_texture);

        auto *command = Allocate<OpenGLCommandSetRenderTexture>(OpenGLCommandType::SetRenderTexture);
        command->render_texture_id = render_texture->GetResourceId();
    }

    void OpenGLCommandBuffer::Blit(RenderTexture *destination, RenderTexture *source) {
        // We explicitly allow either destination or source to be null, so that we may refer to the default framebuffer.
        HYP_ASSERT(destination != nullptr || source != nullptr);

        auto *command = Allocate<OpenGLCommandBlit>(OpenGLCommandType::Blit);
        if (destination != nullptr) {
            command->destination_id = destination->GetResourceId();
            command->destination_width = destination->GetWidth();
            command->destination_height = destination->GetHeight();
        } else {
            command->destination_id = 0;
            command->destination_width = Display::GetWidth();
            command->destination_height = Display::GetHeight();
        }
        if (source != nullptr) {
            command->source_id = source->GetResourceId();
            command->source_width = source->GetWidth();
            command->source_height = source->GetHeight();
        } else {
            command->source_id = 0;
            command->source_width = Display::GetWidth();
            command->source_height = Display::GetHeight();
        }
    }

    void OpenGLCommandBuffer::DrawMesh(Mesh *mesh, Material *material, uint32 sub_mesh_index) {
        HYP_ASSERT(mesh);
        HYP_ASSERT(material);

        auto *command = Allocate<OpenGLCommandDrawMesh>(OpenGLCommandType::DrawMesh);
        command->mesh_id = mesh->GetResourceId();
        command->material_id = material->GetResourceId();
        command->sub_mesh_index = sub_mesh_index;
    }

}