#include "hyppch.hpp"

#include "hyperion/rendering/shader.hpp"

#include <filesystem>

#include "hyperion/driver/opengl/opengl_shader.hpp"
#include "hyperion/core/io/file_utilities.hpp"

namespace Hyperion::Rendering {

    Ref<Shader> Shader::Create(const String &name, const String &source) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLShader>(name, source);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Ref<Shader> Shader::Create(const String &name, const String &vertex_source, const String &fragment_source) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLShader>(name, vertex_source, fragment_source);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    ShaderType Shader::ShaderTypeFromString(const String &string) {
        if (string == "vertex") {
            return ShaderType::Vertex;
        } else if (string == "fragment") {
            return ShaderType::Fragment;
        } else {
            HYP_ASSERT_MESSAGE(false, "Failed to get shader type from string!"); return ShaderType::None;
        }
    }

}