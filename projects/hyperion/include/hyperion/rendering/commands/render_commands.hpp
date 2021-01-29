#pragma once

#include "hyperion/rendering/render_driver.hpp"

// NOTE: Everything inside a render command has to be trivially destructable (POD).
// The reason is that only their storage gets deallocated and the destructor will never be called.
// Using a type which is not trivially destructable would be a memory leak!

namespace Hyperion::Rendering {

    // Those specify all 'normal' render commands.
    // Immediate render commands are handled seperately.
    enum class RenderCommandType {
        Exit,

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

        CreateMesh,
        DrawMesh,
        DestroyMesh,
    };

    // This is a generic render command containing just an id.
    // Can be reused for different command types.
    struct RenderCommandId {
        ResourceId id;
    };

    struct RenderCommandClear {
        ClearFlags clear_flags;
        Color color;
    };

    struct RenderCommandViewport {
        Viewport viewport;
    };

    struct RenderCommandSetRasterizerState {
        RasterizerState rasterizer_state;
    };

    struct RenderCommandCreateShader {
        ResourceId shader_id;
        ShaderDescriptor descriptor;
    };

    struct RenderCommandCreateTexture {
        ResourceId texture_id;
        TextureDescriptor descriptor;
    };

    struct RenderCommandCreateMaterial {
        ResourceId material_id;
        MaterialDescriptor descriptor;
    };

    struct RenderCommandSetMaterialProperty {
        ResourceId material_id;
        MaterialProperty property;
    };

    struct RenderCommandCreateMesh {
        ResourceId mesh_id;
        MeshDescriptor descriptor;
    };

    struct RenderCommandDrawMesh {
        ResourceId mesh_id;
        ResourceId material_id;
        uint32 sub_mesh_index;
    };

}