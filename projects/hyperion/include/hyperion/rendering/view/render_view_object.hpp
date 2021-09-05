#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderViewObjectData {
        // Material *material;
        // Mesh *mesh;
        // uint32 sub_mesh_index;
        
        // Matrix4x4 local_to_world;
    };

    class RenderViewObject {
    public:
        RenderViewObject(const RenderViewObjectData &data) : m_data(data) {}
    private:
        RenderViewObjectData m_data;
    };

}