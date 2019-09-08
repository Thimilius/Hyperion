#include "hyppch.hpp"

#include "hyperion/rendering/shader.hpp"

#include "hyperion/platform/opengl/opengl_shader.hpp"

#include "hyperion/system/file_utilities.hpp"

namespace Hyperion::Rendering {

    TRef<CShader> CShader::Create(const TString &source) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return std::make_shared<COpenGLShader>(source);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    TRef<CShader> CShader::Create(const TString &vertex_source, const TString &fragment_source) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return std::make_shared<COpenGLShader>(vertex_source, fragment_source);
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

    TRef<CShader> CShaderLibrary::Load(const TString &name, const TString &filepath) {
        auto &source = CFileUtilities::ReadFile(filepath);
        auto &shader = CShader::Create(source);
        Add(name, shader);
        return shader;
    }

    void CShaderLibrary::Add(const TString &name, const TRef<CShader> &shader) {
        HYP_ASSERT_MESSAGE(m_shaders.find(name) == m_shaders.end(), "Shader already in library!");
        m_shaders[name] = shader;
    }

    TRef<CShader> CShaderLibrary::Get(const TString &name) {
        HYP_ASSERT_MESSAGE(m_shaders.find(name) != m_shaders.end(), "Can not find shader in library!");
        return m_shaders[name];
    }

}