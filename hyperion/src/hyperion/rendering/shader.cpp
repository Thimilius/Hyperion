#include "hyppch.hpp"

#include "hyperion/rendering/shader.hpp"

#include "hyperion/platform/opengl/opengl_shader.hpp"

namespace Hyperion::Rendering {

    CShader *CShader::Create(const TString &vertex_source, const TString &fragment_source) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return new COpenGLShader(vertex_source, fragment_source);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}