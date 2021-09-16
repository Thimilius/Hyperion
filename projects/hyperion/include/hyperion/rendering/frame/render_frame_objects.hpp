#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderFrameObject {
        Matrix4x4 local_to_world;
    };

    struct RenderFrameMeshObject : public RenderFrameObject {
        Mesh *mesh;
        uint32 sub_mesh_index;

        Material *material;

        RenderLayerMask layer_mask;
    };

    struct RenderFrameSpriteObject : public RenderFrameObject {
        Color color;
    };

}