#pragma once

#include <glad/glad.h>

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class OpenGLRenderDriver : public IRenderDriver {
    public:
        void Clear(ClearFlags clear_mask, Color color) override;

        void CreateMesh(ResourceId id, const Vector<VertexMesh> &vertices, const Vector<u32> &indices) override;
        void FreeMesh(ResourceId id) override;
    private:
        static u32 GetGLClearFlags(ClearFlags clear_mask);
    private:
        struct OpenGLMesh {
            ResourceId id;

            // Vertex and index buffer have to be next to each other to support efficient creation and destruction
            GLuint vertex_buffer;
            GLuint index_buffer;

            GLuint vertex_array;
        };
        inline static Map<ResourceId, OpenGLMesh> s_meshes;
    };

}