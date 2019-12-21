#include "hyppch.hpp"

#include "hyperion/rendering/vertex_array.hpp"

#include "hyperion/driver/opengl/opengl_vertex_array.hpp"

namespace Hyperion::Rendering {

    Ref<VertexArray> VertexArray::Create() {
        switch (RenderAPI::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLVertexArray>();
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}