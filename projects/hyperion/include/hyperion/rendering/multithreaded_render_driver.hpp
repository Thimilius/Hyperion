#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class MultithreadedRenderDriver final : public IRenderDriver {
    public:
        void Clear(ClearFlags clear_flags, Color color) override;

        void CreateShader(ResourceId id, const String &vertex, const String &fragment) override;
        void FreeShader(ResourceId id) override;

        void CreateMesh(ResourceId id, const Vector<VertexMesh> &vertices, const Vector<u32> &indices) override;
        void FreeMesh(ResourceId id) override;

        void DrawIndexed(ResourceId shader_id, ResourceId mesh_id) override;
    };

}