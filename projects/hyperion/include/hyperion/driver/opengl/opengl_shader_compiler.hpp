#pragma once

#include <glad/glad.h>

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    struct OpenGLShaderCompilationResult {
        bool success;

        GLuint program;
    };

    class OpenGLShaderCompiler {
    public:
        static OpenGLShaderCompilationResult Compile(const char *vertex_source, const char *fragment_source);
    private:
        OpenGLShaderCompiler() = delete;
        ~OpenGLShaderCompiler() = delete;
    };

}