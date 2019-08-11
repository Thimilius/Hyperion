#include "hyppch.hpp"

#include "hyperion/rendering/vertex_array.hpp"

#include "hyperion/platform/opengl/opengl_vertex_array.hpp"

namespace Hyperion::Rendering {

    CVertexArray *CVertexArray::Create() {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return new COpenGLVertexArray();
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}