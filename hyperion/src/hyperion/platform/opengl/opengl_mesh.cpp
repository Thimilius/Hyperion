#include "hyppch.hpp"

#include "opengl_mesh.hpp"

namespace Hyperion::Rendering {

    COpenGLMesh::COpenGLMesh(const TRef<CVertexArray> &vertex_array) {
        m_vertex_array = vertex_array;
    }

}