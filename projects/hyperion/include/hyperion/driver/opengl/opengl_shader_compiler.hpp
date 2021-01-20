#pragma once

#include <glad/glad.h>

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    struct OpenGLShaderCompilationResult {
        bool succes = false;
        GLuint program;
    };

    class OpenGLShaderCompiler {
    public:
        static OpenGLShaderCompilationResult Compile(const char *vertex, const char *fragment);
    private:
        OpenGLShaderCompiler() = delete;
        ~OpenGLShaderCompiler() = delete;
    };

}