//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/threading/render_thread_command_executor.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/render_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    uint64 RenderThreadCommandExecutor::Execute(IRenderDriver *render_driver, RenderThreadCommandType command_type, void *render_thread_command) {
        switch (command_type) {
            case RenderThreadCommandType::Clear: {
                auto command = reinterpret_cast<RenderThreadCommandClear *>(render_thread_command);
                render_driver->Clear(command->clear_flags, command->color);
                return sizeof(*command);
            }
            case RenderThreadCommandType::SetViewport: {
                auto command = reinterpret_cast<RenderThreadCommandSetViewport *>(render_thread_command);
                render_driver->SetViewport(command->viewport);
                return sizeof(*command);
            }
            case RenderThreadCommandType::SetRasterizerState: {
                auto command = reinterpret_cast<RenderThreadCommandSetRasterizerState *>(render_thread_command);
                render_driver->SetRasterizerState(command->rasterizer_state);
                return sizeof(*command);
            }
            case RenderThreadCommandType::SetCameraData: {
                auto command = reinterpret_cast<RenderThreadCommandSetCameraData *>(render_thread_command);
                render_driver->SetCameraData(command->camera_data);
                return sizeof(*command);
            }
            case RenderThreadCommandType::CreateShader: {
                auto command = reinterpret_cast<RenderThreadCommandCreateOrRecompileShader *>(render_thread_command);
                RenderThreadCommandQueueHelper helper(command);
                helper.Read(command->descriptor.source_vertex);
                helper.Read(command->descriptor.source_fragment);

                render_driver->CreateShader(command->shader_id, command->descriptor);
                
                uint64 extra_size = command->descriptor.source_vertex.size + command->descriptor.source_fragment.size;
                return sizeof(*command) + extra_size;
            }
            case RenderThreadCommandType::RecompileShader: {
                auto command = reinterpret_cast<RenderThreadCommandCreateOrRecompileShader *>(render_thread_command);
                RenderThreadCommandQueueHelper helper(command);
                helper.Read(command->descriptor.source_vertex);
                helper.Read(command->descriptor.source_fragment);

                render_driver->RecompileShader(command->shader_id, command->descriptor);

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
            case RenderThreadCommandType::ResizeRenderTexture: {
                auto command = reinterpret_cast<RenderThreadCommandResizeRenderTexture *>(render_thread_command);
                render_driver->ResizeRenderTexture(command->render_texture_id, command->width, command->height, command->mipmap_count);
                return sizeof(*command);
            }
            case RenderThreadCommandType::SetRenderTexture: {
                auto command = reinterpret_cast<RenderThreadCommandId *>(render_thread_command);
                render_driver->SetRenderTexture(command->id);
                return sizeof(*command);
            }
            case RenderThreadCommandType::BlitRenderTexture: {
                auto command = reinterpret_cast<RenderThreadCommandBlitRenderTexture *>(render_thread_command);
                render_driver->BlitRenderTexture(command->destination_id, command->source_id);
                return sizeof(*command);
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
            case RenderThreadCommandType::DrawMesh: {
                auto command = reinterpret_cast<RenderThreadCommandDrawMesh *>(render_thread_command);
                render_driver->DrawMesh(command->mesh_id, command->model_matrix, command->material_id, command->sub_mesh_index);
                return sizeof(*command);
            }
            case RenderThreadCommandType::DestroyMesh: {
                auto command = reinterpret_cast<RenderThreadCommandId *>(render_thread_command);
                render_driver->DestroyMesh(command->id);
                return sizeof(*command);
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    //--------------------------------------------------------------
    uint64 RenderThreadCommandExecutor::ExecuteQuery(IRenderDriver *render_driver, RenderThreadQueryCommandType command_type, void *render_thread_query_command) {
        switch (command_type) {
            case RenderThreadQueryCommandType::GetRenderTextureSubData: {
                auto query_command = reinterpret_cast<RenderThreadQueryCommandGetRenderTextureSubData *>(render_thread_query_command);
                render_driver->GetRenderTextureSubData(query_command->render_texture_id, query_command->attachment_index, query_command->region, query_command->buffer, nullptr);
                return sizeof(*query_command);
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}