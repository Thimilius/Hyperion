#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/render_driver.hpp"

// NOTE: Everything inside a normal render thread command has to be trivially destructable (POD).
// The reason is that only their storage gets deallocated and the destructor will never be called.
// Using a type which is not trivially destructable would be a memory leak!

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    // Those specify all 'normal' render thread commands.
    // Render thread query commands are handled seperately.
    enum class RenderThreadCommandType {
        Exit,

        Clear,
        SetViewport,
        SetRasterizerState,
        SetCameraData,

        CreateShader,
        RecompileShader,
        DestroyShader,

        CreateTexture,
        DestroyTexture,

        CreateMaterial,
        SetMaterialProperty,
        DestroyMaterial,

        CreateRenderTexture,
        ResizeRenderTexture,
        SetRenderTexture,
        BlitRenderTexture,
        DestroyRenderTexture,

        CreateMesh,
        SetMeshData,
        DrawMesh,
        DestroyMesh,
    };

    // This is a generic render thread command containing just an id.
    // Can be reused for different command types.
    struct RenderThreadCommandId {
        ResourceId id;
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

    struct RenderThreadCommandSetCameraData {
        CameraData camera_data;
    };

    struct RenderThreadCommandCreateOrRecompileShader {
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

    struct RenderThreadCommandResizeRenderTexture {
        ResourceId render_texture_id;
        uint32 width;
        uint32 height;
        uint32 mipmap_count;
    };

    struct RenderThreadCommandBlitRenderTexture {
        ResourceId destination_id;
        ResourceId source_id;
    };

    struct RenderThreadCommandCreateMesh {
        ResourceId mesh_id;
        MeshDescriptor descriptor;
    };

    struct RenderThreadCommandSetMeshData {
        ResourceId mesh_id;
        MeshDataDescriptor descriptor;
    };

    struct RenderThreadCommandDrawMesh {
        ResourceId mesh_id;
        Mat4 model_matrix;
        ResourceId material_id;
        uint32 sub_mesh_index;
    };

    // NOTE: Render thread query commands are special commands that get executed by the Render Thread at the end of the frame.
    // They execute synchronously, meaning the Main Thread waits for the Render Thread to finish executing.
    // Their primary purpose is to return data from the Render Thread back to the Main Thread.

    enum class RenderThreadQueryCommandType {
        GetRenderTextureSubData,
    };

    struct RenderThreadQueryCommandGetRenderTextureSubData {
        ResourceId render_texture_id;
        uint32 attachment_index;
        RectInt region;
        Vector<uint8> *buffer;
        GetRenderTextureSubDataCallback callback;
    };

}