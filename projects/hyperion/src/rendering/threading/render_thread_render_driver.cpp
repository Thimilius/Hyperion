#include "hyppch.hpp"

#include "hyperion/rendering/threading/render_thread_render_driver.hpp"

#include "hyperion/core/threading/synchronization.hpp"
#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    RenderThreadRenderDriver::RenderThreadRenderDriver(IRenderDriver *backend_render_driver) {
        m_backend_render_driver = backend_render_driver;
    }

    CommandBuffer *RenderThreadRenderDriver::CreateCommandBuffer() {
        return m_backend_render_driver->CreateCommandBuffer();
    }

    void RenderThreadRenderDriver::ExecuteCommandBuffer(CommandBuffer *command_buffer) {
        RenderThreadCommandExecuteCommandBuffer *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandExecuteCommandBuffer>(RenderThreadCommandType::ExecuteCommandBuffer);
        command->command_buffer = m_backend_render_driver->CopyCommandBuffer(command_buffer);
    }

    CommandBuffer *RenderThreadRenderDriver::CopyCommandBuffer(CommandBuffer *command_buffer) {
        return m_backend_render_driver->CopyCommandBuffer(command_buffer);
    }

    void RenderThreadRenderDriver::DestroyCommandBuffer(CommandBuffer *command_buffer) {
        m_backend_render_driver->DestroyCommandBuffer(command_buffer);
    }

    void RenderThreadRenderDriver::Clear(ClearFlags clear_flags, Color color) {
        RenderThreadCommandClear *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandClear>(RenderThreadCommandType::Clear);
        command->clear_flags = clear_flags;
        command->color = color;
    }

    void RenderThreadRenderDriver::SetViewport(const Viewport &viewport) {
        RenderThreadCommandSetViewport *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandSetViewport>(RenderThreadCommandType::SetViewport);
        command->viewport = viewport;
    }

    void RenderThreadRenderDriver::SetRasterizerState(const RasterizerState &rasterizer_state) {
        RenderThreadCommandSetRasterizerState *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandSetRasterizerState>(RenderThreadCommandType::SetRasterizerState);
        command->rasterizer_state = rasterizer_state;
    }

    void RenderThreadRenderDriver::CreateShader(ResourceId shader_id, const ShaderDescriptor &descriptor) {
        uint64 extra_size = descriptor.source_vertex.size + descriptor.source_fragment.size;
        RenderThreadCommandCreateShader *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandCreateShader>(RenderThreadCommandType::CreateShader, extra_size);
        command->shader_id = shader_id;
        command->descriptor = descriptor;

        RenderThreadCommandQueueHelper<RenderThreadCommandCreateShader> helper(command);
        helper.Write(descriptor.source_vertex);
        helper.Write(descriptor.source_fragment);
    }

    void RenderThreadRenderDriver::DestroyShader(ResourceId shader_id) {
        RenderThreadCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandId>(RenderThreadCommandType::DestroyShader);
        command->id = shader_id;
    }

    void RenderThreadRenderDriver::CreateTexture(ResourceId texture_id, const TextureDescriptor &descriptor) {
        uint64 extra_size = descriptor.pixels.size;
        RenderThreadCommandCreateTexture *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandCreateTexture>(RenderThreadCommandType::CreateTexture, extra_size);
        command->texture_id = texture_id;
        command->descriptor = descriptor;

        RenderThreadCommandQueueHelper<RenderThreadCommandCreateTexture> helper(command);
        helper.Write(descriptor.pixels);
    }

    void RenderThreadRenderDriver::GetTextureData(ResourceId texture_id, Vector<uint8> &data) {
        // NOTE: Getting the data of a texture is an immediate render command and needs to be handled appropriately.
        ImmediateRenderThreadCommandGetTextureData command = { };
        command.texture_id = texture_id;
        command.data = &data;

        ImmediateRenderThreadCommand &immediate_command = RenderEngine::GetImmediateCommand();
        immediate_command.type = ImmediateRenderThreadCommandType::GetTextureData;
        immediate_command.command = command;

        RenderEngine::SetImmediateCommandPending();
        Synchronization::WaitForImmediateCommandDone();
    }

    void RenderThreadRenderDriver::DestroyTexture(ResourceId texture_id) {
        RenderThreadCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandId>(RenderThreadCommandType::DestroyTexture);
        command->id = texture_id;
    }

    void RenderThreadRenderDriver::CreateMaterial(ResourceId material_id, const MaterialDescriptor &descriptor) {
        RenderThreadCommandCreateMaterial *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandCreateMaterial>(RenderThreadCommandType::CreateMaterial);
        command->material_id = material_id;
        command->descriptor = descriptor;
    }

    void RenderThreadRenderDriver::SetMaterialProperty(ResourceId material_id, const MaterialProperty &property) {
        RenderThreadCommandSetMaterialProperty *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandSetMaterialProperty>(RenderThreadCommandType::SetMaterialProperty);
        command->material_id = material_id;
        command->property = property;
    }

    void RenderThreadRenderDriver::DestroyMaterial(ResourceId material_id) {
        RenderThreadCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandId>(RenderThreadCommandType::DestroyMaterial);
        command->id = material_id;
    }

    void RenderThreadRenderDriver::CreateRenderTexture(ResourceId render_texture_id, const RenderTextureDescriptor &descriptor) {
        uint64 extra_size = descriptor.attachments.size;
        RenderThreadCommandCreateRenderTexture *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandCreateRenderTexture>(RenderThreadCommandType::CreateRenderTexture, extra_size);
        command->render_texture_id = render_texture_id;
        command->descriptor = descriptor;

        RenderThreadCommandQueueHelper<RenderThreadCommandCreateRenderTexture> helper(command);
        helper.Write(descriptor.attachments);
    }

    void RenderThreadRenderDriver::DestroyRenderTexture(ResourceId render_texture_id) {
        RenderThreadCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandId>(RenderThreadCommandType::DestroyRenderTexture);
        command->id = render_texture_id;
    }

    void RenderThreadRenderDriver::CreateMesh(ResourceId mesh_id, const MeshDescriptor &descriptor) {
        uint64 extra_size = descriptor.sub_meshes.size + descriptor.vertex_format.attributes.size + descriptor.vertices.size + descriptor.indices.size;
        RenderThreadCommandCreateMesh *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandCreateMesh>(RenderThreadCommandType::CreateMesh, extra_size);
        command->mesh_id = mesh_id;
        command->descriptor = descriptor;

        RenderThreadCommandQueueHelper helper(command);
        helper.Write(descriptor.sub_meshes);
        helper.Write(descriptor.vertex_format.attributes);
        helper.Write(descriptor.vertices);
        helper.Write(descriptor.indices);
    }

    void RenderThreadRenderDriver::DrawMesh(ResourceId mesh_id, ResourceId material_id, uint32 sub_mesh_index) {
        RenderThreadCommandDrawMesh *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandDrawMesh>(RenderThreadCommandType::DrawMesh);
        command->mesh_id = mesh_id;
        command->material_id = material_id;
        command->sub_mesh_index = sub_mesh_index;
    }

    void RenderThreadRenderDriver::DestroyMesh(ResourceId mesh_id) {
        RenderThreadCommandId *command = RenderEngine::GetCommandQueue().Allocate<RenderThreadCommandId>(RenderThreadCommandType::DestroyMesh);
        command->id = mesh_id;
    }

}