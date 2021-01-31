#pragma once

#include <variant>

#include "hyperion/rendering/render_driver.hpp"

// NOTE: Everything inside a (normal) render thread command has to be trivially destructable (POD).
// The reason is that only their storage gets deallocated and the destructor will never be called.
// Using a type which is not trivially destructable would be a memory leak!

namespace Hyperion::Rendering {

    // Those specify all 'normal' render thread commands.
    // Immediate render thread commands are handled seperately.
    enum class RenderThreadCommandType {
        Exit,

        ExecuteCommandBuffer,

        Clear,
        SetViewport,
        SetRasterizerState,

        CreateShader,
        DestroyShader,

        CreateTexture,
        DestroyTexture,

        CreateMaterial,
        SetMaterialProperty,
        DestroyMaterial,

        CreateRenderTexture,
        DestroyRenderTexture,

        CreateMesh,
        DrawMesh,
        DestroyMesh,
    };

    // This is a generic render thread command containing just an id.
    // Can be reused for different command types.
    struct RenderThreadCommandId {
        ResourceId id;
    };

    struct RenderThreadCommandExecuteCommandBuffer {
        CommandBuffer *command_buffer;
    };

    struct RenderThreadCommandClear {
        ClearFlags clear_flags;
        Color color;
    };

    struct RenderThreadCommandSetViewport {
        Viewport viewport;
    };

    struct RenderThreadCommandSetRasterizerState {
        RasterizerState rasterizer_state;
    };

    struct RenderThreadCommandCreateShader {
        ResourceId shader_id;
        ShaderDescriptor descriptor;
    };

    struct RenderThreadCommandCreateTexture {
        ResourceId texture_id;
        TextureDescriptor descriptor;
    };

    struct RenderThreadCommandCreateMaterial {
        ResourceId material_id;
        MaterialDescriptor descriptor;
    };

    struct RenderThreadCommandSetMaterialProperty {
        ResourceId material_id;
        MaterialProperty property;
    };

    struct RenderThreadCommandCreateRenderTexture {
        ResourceId render_texture_id;
        RenderTextureDescriptor descriptor;
    };

    struct RenderThreadCommandCreateMesh {
        ResourceId mesh_id;
        MeshDescriptor descriptor;
    };

    struct RenderThreadCommandDrawMesh {
        ResourceId mesh_id;
        ResourceId material_id;
        uint32 sub_mesh_index;
    };

    // NOTE: Immediate render thread commands are special commands that get executed immediately by the Render Thread.
    // They are synchronized, meaning the Main Thread waits for the Render Thread to execute them.
    // Their primary purpose is to return data from the Render Thread back to the Main Thread.

    enum class ImmediateRenderThreadCommandType {
        GetTextureData
    };

    struct ImmediateRenderThreadCommandGetTextureData {
        ResourceId texture_id;
        Vector<uint8> *data;
    };

    struct ImmediateRenderThreadCommand {
        ImmediateRenderThreadCommandType type;

        std::variant<ImmediateRenderThreadCommandGetTextureData> command; // TODO: Can we get rid of the ugly variant here?
    };

}