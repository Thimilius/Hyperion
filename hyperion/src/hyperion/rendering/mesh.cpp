#include "hyppch.hpp"

#include "hyperion/rendering/mesh.hpp"

#include "hyperion/platform/opengl/opengl_mesh.hpp"

namespace Hyperion::Rendering {

    TRef<CMesh> CMesh::Create(const TRef<CVertexArray> &vertex_array) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return std::make_shared<COpenGLMesh>(vertex_array);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    TRef<CMesh> CMesh::CreatePlane() {
        return nullptr;
    }

}