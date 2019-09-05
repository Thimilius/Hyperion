#include "hyppch.hpp"

#include "hyperion/rendering/shader.hpp"

#include "hyperion/platform/opengl/opengl_shader.hpp"

namespace Hyperion::Rendering {

    CShader *CShader::Create(const TString &source) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return new COpenGLShader(source);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    CShader *CShader::Create(const TString &vertex_source, const TString &fragment_source) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return new COpenGLShader(vertex_source, fragment_source);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    EShaderType CShader::ShaderTypeFromString(const TString &string) {
        if (string == "vertex") {
            return EShaderType::Vertex;
        } else if (string == "fragment") {
            return EShaderType::Fragment;
        } else {
            HYP_ASSERT_MESSAGE(false, "Failed to get shader type from string!"); return EShaderType::None;
        }
    }

}