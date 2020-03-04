#include "hyppch.hpp"

#include "hyperion/rendering/shader.hpp"

#include <filesystem>

#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/driver/opengl/opengl_shader.hpp"
#include "hyperion/core/io/file_utilities.hpp"

namespace Hyperion::Rendering {

    Ref<Shader> Shader::Create(const String &name, const String &source) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLShader>(name, source);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}