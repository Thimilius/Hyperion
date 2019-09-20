#pragma once

#include "hyperion/rendering/mesh.hpp"

namespace Hyperion::Rendering {

    class COpenGLMesh : public CMesh {
    public:
        COpenGLMesh(const TRef<CVertexArray> &vertex_array);
    };

}