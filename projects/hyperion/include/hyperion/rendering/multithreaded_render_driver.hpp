#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class MultithreadedRenderDriver final : public IRenderDriver {
    public:
        void Clear(ClearFlags clear_flags, Color color) override;

        void CreateMesh(ResourceId id, const Vector<VertexMesh> &vertices, const Vector<u32> &indices) override;
        void FreeMesh(ResourceId id) override;
    };

}