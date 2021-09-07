#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderFrameObjectData {
        // Material *material;
        // Mesh *mesh;
        // uint32 sub_mesh_index;
        
        // Matrix4x4 local_to_world;
    };

    class RenderFrameObject {
    public:
        RenderFrameObjectData &GetData() { return m_data; }
        const RenderFrameObjectData &GetData() const { return m_data; }
    private:
        RenderFrameObjectData m_data;
    };

}