#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderFrameObjectData {
        // Material *material;
        // Mesh *mesh;
        // uint32 sub_mesh_index;
        
        Matrix4x4 local_to_world;

        // TEMP: This is just so we somehow get the stuff accross.
        Mesh *mesh;
        Texture2D *texture;
    };

    class RenderFrameObject {
    public:
        RenderFrameObjectData &GetData() { return m_data; }
        const RenderFrameObjectData &GetData() const { return m_data; }
    private:
        RenderFrameObjectData m_data;
    };

}