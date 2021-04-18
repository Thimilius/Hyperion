#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
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