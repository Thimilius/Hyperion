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
            case RenderCommandType::SetViewport: {
                auto render_command = reinterpret_cast<RenderCommandViewport *>(command);
                render_driver->SetViewport(render_command->viewport);
                return sizeof(*render_command);
            }
            case RenderCommandType::SetRasterizerState: {
                auto render_command = reinterpret_cast<RenderCommandSetRasterizerState *>(command);
                render_driver->SetRasterizerState(render_command->rasterizer_state);
                return sizeof(*render_command);
            }
            case RenderCommandType::CreateShader: {
                auto render_command = reinterpret_cast<RenderCommandCreateShader *>(command);
                RenderCommandQueueHelper helper(render_command);
                helper.Read(render_command->descriptor.source_vertex);
                helper.Read(render_command->descriptor.source_fragment);

                render_driver->CreateShader(render_command->shader_id, render_command->descriptor);
                
                uint64 extra_size = render_command->descriptor.source_vertex.size + render_command->descriptor.source_fragment.size;
                return sizeof(*render_command) + extra_size;
            }
            case RenderCommandType::DestroyShader: {
                auto render_command = reinterpret_cast<RenderCommandId *>(command);
                render_driver->DestroyShader(render_command->id);
                return sizeof(*render_command);
            }
            case RenderCommandType::CreateMesh: {
                auto render_command = reinterpret_cast<RenderCommandCreateMesh *>(command);
                RenderCommandQueueHelper helper(render_command);
                helper.Read(render_command->descriptor.sub_meshes);
                helper.Read(render_command->descriptor.vertex_format.attributes);
                helper.Read(render_command->descriptor.vertices);
                helper.Read(render_command->descriptor.indices);

                render_driver->CreateMesh(render_command->mesh_id, render_command->descriptor);

                uint64 extra_size = render_command->descriptor.sub_meshes.size + render_command->descriptor.vertex_format.attributes.size + render_command->descriptor.vertices.size + render_command->descriptor.indices.size;
                return sizeof(*render_command) + extra_size;
            }
            case RenderCommandType::DrawMesh: {
                auto render_command = reinterpret_cast<RenderCommandDrawMesh *>(command);
                render_driver->DrawMesh(render_command->mesh_id, render_command->material_id, render_command->sub_mesh_index);
                return sizeof(*render_command);
            }
            case RenderCommandType::DestroyMesh: {
                auto render_command = reinterpret_cast<RenderCommandId *>(command);
                render_driver->DestroyMesh(render_command->id);
                return sizeof(*render_command);
            }
            case RenderCommandType::CreateTexture: {
                auto render_command = reinterpret_cast<RenderCommandCreateTexture *>(command);
                RenderCommandQueueHelper helper(render_command);
                helper.Read(render_command->descriptor.pixels);

                render_driver->CreateTexture(render_command->texture_id, render_command->descriptor);
                
                uint64 extra_size = render_command->descriptor.pixels.size;
                return sizeof(*render_command) + extra_size;
            }
            case RenderCommandType::DestroyTexture: {
                auto render_command = reinterpret_cast<RenderCommandId *>(command);
                render_driver->DestroyTexture(render_command->id);
                return sizeof(*render_command);
            }
            case RenderCommandType::CreateMaterial: {
                auto render_command = reinterpret_cast<RenderCommandCreateMaterial *>(command);
                render_driver->CreateMaterial(render_command->material_id, render_command->descriptor);
                return sizeof(*render_command);
            }
            case RenderCommandType::SetMaterialProperty: {
                auto render_command = reinterpret_cast<RenderCommandSetMaterialProperty *>(command);
                render_driver->SetMaterialProperty(render_command->material_id, render_command->property);
                return sizeof(*render_command);
            }
            case RenderCommandType::DestroyMaterial: {
                auto render_command = reinterpret_cast<RenderCommandId *>(command);
                render_driver->DestroyMaterial(render_command->id);
                return sizeof(*render_command);
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    void RenderCommandExecutor::ExecuteImmediateRenderCommand(ImmediateRenderCommand &immediate_render_command, IRenderDriver *render_driver) {
        switch (immediate_render_command.type) {
            case ImmediateRenderCommandType::GetTextureData: {
                auto &command = std::get<ImmediateRenderCommandGetTextureData>(immediate_render_command.command);
                render_driver->GetTextureData(command.texture_id, *command.data);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

}