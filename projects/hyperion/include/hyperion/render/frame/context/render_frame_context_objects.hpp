#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderFrameContextObject {
        Matrix4x4 local_to_world;
        Vector3 position;
    };

    struct RenderFrameContextObjectMesh : public RenderFrameContextObject {
        Mesh *mesh;
        uint32 sub_mesh_index;

        Material *material;

        LayerMask layer_mask;
    };

    struct RenderFrameContextObjectSprite : public RenderFrameContextObject {
        Color color;
    };

}