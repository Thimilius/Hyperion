#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct OpenGLGraphicsShaderCompilationResult {
        bool8 success;

        GLuint program;
    };

    class OpenGLGraphicsShaderCompiler {
    private:
        OpenGLGraphicsShaderCompiler() = delete;
        ~OpenGLGraphicsShaderCompiler() = delete;
    public:
        static OpenGLGraphicsShaderCompilationResult Compile(const char *vertex_source, const char *fragment_source);
    };

}