#include "hyppch.hpp"

#include "hyperion/rendering/shader.hpp"

#include <filesystem>

#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/driver/opengl/opengl_shader.hpp"

namespace Hyperion::Rendering {

    Shader *Shader::Create(const String &name, const String &source) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return new OpenGLShader(name, source);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}