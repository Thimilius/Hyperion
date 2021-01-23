#include "hyppch.hpp"

#include "hyperion/rendering/commands/render_command_executor.hpp"

#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    uint64 RenderCommandExecutor::ExecuteRenderCommand(RenderCommandType command_type, void *command, IRenderDriver *render_driver) {
        switch (command_type) {
            case RenderCommandType::Clear: {
                auto render_command = reinterpret_cast<RenderCommandClear *>(command);
                render_driver->Clear(render_command->clear_flags, render_command->color);
                return sizeof(*render_command);
            }
            case RenderCommandType::Viewport: {
                auto render_command = reinterpret_cast<RenderCommandViewport *>(command);
                render_driver->Viewport(render_command->viewport);
                return sizeof(*render_command);
            }
            case RenderCommandType::SetRasterizerState: {
                auto render_command = reinterpret_cast<RenderCommandSetRasterizerState *>(command);
                render_driver->SetRasterizerState(render_command->rasterizer_state);
                return sizeof(*render_command);
            }
            case RenderCommandType::CreateShader: {
                auto render_command = reinterpret_cast<RenderCommandCreateShader *>(command);

                uint64 extra_size = render_command->descriptor.source_vertex.size + render_command->descriptor.source_fragment.size;
                uint8 *data = reinterpret_cast<uint8 *>(render_command + 1);
                render_command->descriptor.source_vertex.data = reinterpret_cast<char *>(data);
                render_command->descriptor.source_fragment.data = reinterpret_cast<char *>(data) + render_command->descriptor.source_vertex.size;
                
                render_driver->CreateShader(render_command->id, render_command->descriptor);
                return sizeof(*render_command) + extra_size;
            }
            case RenderCommandType::DestroyShader: {
                auto render_command = reinterpret_cast<RenderCommandId *>(command);
                render_driver->DestroyShader(render_command->id);
                return sizeof(*render_command);
            }
            case RenderCommandType::CreateMesh: {
                auto render_command = reinterpret_cast<RenderCommandCreateMesh *>(command);

                uint64 extra_size = render_command->descriptor.sub_meshes.size + render_command->descriptor.vertex_format.attributes.size + render_command->descriptor.vertices.size + render_command->descriptor.indices.size;
                uint8 *data = reinterpret_cast<uint8 *>(render_command + 1);
                render_command->descriptor.sub_meshes.data = reinterpret_cast<SubMesh *>(data);
                render_command->descriptor.vertex_format.attributes.data = reinterpret_cast<VertexAttributeDescriptor *>(data + render_command->descriptor.sub_meshes.size);
                render_command->descriptor.vertices.data = data + render_command->descriptor.sub_meshes.size + render_command->descriptor.vertex_format.attributes.size;
                render_command->descriptor.indices.data = data + render_command->descriptor.sub_meshes.size + render_command->descriptor.vertex_format.attributes.size + render_command->descriptor.vertices.size;

                render_driver->CreateMesh(render_command->id, render_command->descriptor);
                return sizeof(*render_command) + extra_size;
            }
            case RenderCommandType::DestroyMesh: {
                auto render_command = reinterpret_cast<RenderCommandId *>(command);
                render_driver->DestroyMesh(render_command->id);
                return sizeof(*render_command);
            }
            case RenderCommandType::CreateTexture: {
                auto render_command = reinterpret_cast<RenderCommandCreateTexture *>(command);

                uint64 extra_size = render_command->descriptor.pixels.size;
                uint8 *data = reinterpret_cast<uint8 *>(render_command + 1);
                render_command->descriptor.pixels.data = data;

                render_driver->CreateTexture(render_command->id, render_command->descriptor);
                return sizeof(*render_command) + extra_size;
            }
            case RenderCommandType::DestroyTexture: {
                auto render_command = reinterpret_cast<RenderCommandId *>(command);
                render_driver->DestroyTexture(render_command->id);
                return sizeof(*render_command);
            }
            case RenderCommandType::CreateMaterial: {
                auto render_command = reinterpret_cast<RenderCommandCreateMaterial *>(command);
                render_driver->CreateMaterial(render_command->id, render_command->descriptor);
                return sizeof(*render_command);
            }
            case RenderCommandType::DestroyMaterial: {
                auto render_command = reinterpret_cast<RenderCommandId *>(command);
                render_driver->DestroyMaterial(render_command->id);
                return sizeof(*render_command);
            }
            case RenderCommandType::DrawIndexed: {
                auto render_command = reinterpret_cast<RenderCommandDrawIndexed *>(command);
                render_driver->DrawIndexed(render_command->mesh_id, render_command->material_id);
                return sizeof(*render_command);
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}