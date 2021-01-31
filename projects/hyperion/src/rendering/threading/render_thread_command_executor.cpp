#include "hyppch.hpp"

#include "hyperion/rendering/threading/render_thread_command_executor.hpp"

#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    uint64 RenderThreadCommandExecutor::Execute(IRenderDriver *render_driver, RenderThreadCommandType command_type, void *render_thread_command) {
        switch (command_type) {
            case RenderThreadCommandType::ExecuteCommandBuffer: {
                auto command = reinterpret_cast<RenderThreadCommandExecuteCommandBuffer *>(render_thread_command);
                render_driver->ExecuteCommandBuffer(command->command_buffer);
                render_driver->DestroyCommandBuffer(command->command_buffer);
                return sizeof(*command);
            }
            case RenderThreadCommandType::CreateShader: {
                auto command = reinterpret_cast<RenderThreadCommandCreateShader *>(render_thread_command);
                RenderThreadCommandQueueHelper helper(command);
                helper.Read(command->descriptor.source_vertex);
                helper.Read(command->descriptor.source_fragment);

                render_driver->CreateShader(command->shader_id, command->descriptor);
                
                uint64 extra_size = command->descriptor.source_vertex.size + command->descriptor.source_fragment.size;
                return sizeof(*command) + extra_size;
            }
            case RenderThreadCommandType::DestroyShader: {
                auto command = reinterpret_cast<RenderThreadCommandId *>(render_thread_command);
                render_driver->DestroyShader(command->id);
                return sizeof(*command);
            }
            case RenderThreadCommandType::CreateTexture: {
                auto command = reinterpret_cast<RenderThreadCommandCreateTexture *>(render_thread_command);
                RenderThreadCommandQueueHelper helper(command);
                helper.Read(command->descriptor.pixels);

                render_driver->CreateTexture(command->texture_id, command->descriptor);
                
                uint64 extra_size = command->descriptor.pixels.size;
                return sizeof(*command) + extra_size;
            }
            case RenderThreadCommandType::DestroyTexture: {
                auto command = reinterpret_cast<RenderThreadCommandId *>(render_thread_command);
                render_driver->DestroyTexture(command->id);
                return sizeof(*command);
            }
            case RenderThreadCommandType::CreateMaterial: {
                auto command = reinterpret_cast<RenderThreadCommandCreateMaterial *>(render_thread_command);
                render_driver->CreateMaterial(command->material_id, command->descriptor);
                return sizeof(*command);
            }
            case RenderThreadCommandType::SetMaterialProperty: {
                auto command = reinterpret_cast<RenderThreadCommandSetMaterialProperty *>(render_thread_command);
                render_driver->SetMaterialProperty(command->material_id, command->property);
                return sizeof(*command);
            }
            case RenderThreadCommandType::DestroyMaterial: {
                auto command = reinterpret_cast<RenderThreadCommandId *>(render_thread_command);
                render_driver->DestroyMaterial(command->id);
                return sizeof(*command);
            }
            case RenderThreadCommandType::CreateRenderTexture: {
                auto command = reinterpret_cast<RenderThreadCommandCreateRenderTexture *>(render_thread_command);
                RenderThreadCommandQueueHelper<RenderThreadCommandCreateRenderTexture> helper(command);
                helper.Read(command->descriptor.attachments);

                render_driver->CreateRenderTexture(command->render_texture_id, command->descriptor);

                uint64 extra_size = command->descriptor.attachments.size;
                return sizeof(*command) + extra_size;
            }
            case RenderThreadCommandType::DestroyRenderTexture: {
                auto command = reinterpret_cast<RenderThreadCommandId *>(render_thread_command);
                render_driver->DestroyRenderTexture(command->id);
                return sizeof(*command);
            }
            case RenderThreadCommandType::CreateMesh: {
                auto command = reinterpret_cast<RenderThreadCommandCreateMesh *>(render_thread_command);
                RenderThreadCommandQueueHelper helper(command);
                helper.Read(command->descriptor.sub_meshes);
                helper.Read(command->descriptor.vertex_format.attributes);
                helper.Read(command->descriptor.vertices);
                helper.Read(command->descriptor.indices);

                render_driver->CreateMesh(command->mesh_id, command->descriptor);

                uint64 extra_size = command->descriptor.sub_meshes.size + command->descriptor.vertex_format.attributes.size + command->descriptor.vertices.size + command->descriptor.indices.size;
                return sizeof(*command) + extra_size;
            }
            case RenderThreadCommandType::DestroyMesh: {
                auto command = reinterpret_cast<RenderThreadCommandId *>(render_thread_command);
                render_driver->DestroyMesh(command->id);
                return sizeof(*command);
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    void RenderThreadCommandExecutor::ExecuteImmediate(IRenderDriver *render_driver, ImmediateRenderThreadCommand &immediate_render_thread_command) {
        switch (immediate_render_thread_command.type) {
            case ImmediateRenderThreadCommandType::GetTextureData: {
                auto &immediate_command = std::get<ImmediateRenderThreadCommandGetTextureData>(immediate_render_thread_command.command);
                render_driver->GetTextureData(immediate_command.texture_id, *immediate_command.data);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

}