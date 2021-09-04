#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct RenderObjectData {
        // Material *material;
        // Mesh *mesh;
        // uint32 sub_mesh_index;
        
        // Matrix4x4 local_to_world;
    };

    class RenderObject {
    public:
        RenderObject(const RenderObjectData &data) : m_data(data) { }
    private:
        RenderObjectData m_data;
    };

}