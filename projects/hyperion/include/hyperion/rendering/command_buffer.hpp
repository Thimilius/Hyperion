#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class CommandBuffer {
    public:
        virtual ~CommandBuffer() = default;

        virtual void ClearCommands() = 0;

        virtual void Clear(ClearFlags clear_flags, Color color) = 0;
        virtual void SetViewport(const Viewport &viewport) = 0;

        virtual void DrawMesh(ResourceId mesh_id, ResourceId material_id, uint32 sub_mesh_index) = 0;
    };

}