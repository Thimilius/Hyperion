#pragma once

#include "hyperion/rendering/render_driver.hpp"

// NOTE: Everything inside a render command has to be trivially destructable.
// The reason is that only their storage gets deallocated and the destructor will never be called.
// Using a type which is not trivially destructable is therefore a memory leak!

namespace Hyperion::Rendering {

    enum class RenderCommandType {
        Exit = -1,

        Clear,
        Viewport,
        SetRasterizerState,

        CreateShader,
        DestroyShader,

        CreateMesh,
        DestroyMesh,

        CreateTexture,
        GetTextureData,
        DestroyTexture,

        CreateMaterial,
        SetMaterialProperty,
        DestroyMaterial,

        DrawIndexed
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

    struct RenderCommandId {
        ResourceId id;
    };

    struct RenderCommandCreateShader {
        ResourceId id;
        ShaderDescriptor descriptor;
    };

    struct RenderCommandCreateMesh {
        ResourceId id;
        MeshDescriptor descriptor;
    };

    struct RenderCommandCreateTexture {
        ResourceId id;
        TextureDescriptor descriptor;
    };

    struct RenderCommandCreateMaterial {
        ResourceId id;
        MaterialDescriptor descriptor;
    };

    struct RenderCommandSetMaterialProperty {
        ResourceId id;
        MaterialProperty property;
    };

    struct RenderCommandDrawIndexed {
        ResourceId mesh_id;
        ResourceId material_id;
    };

    struct RenderCommandImmediate {
        RenderCommandType type;

        ResourceId id;
        Vector<uint8> pixels;
    };

}