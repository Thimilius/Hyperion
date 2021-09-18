#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct OpenGLRenderDriverShaderCompilationResult {
        bool8 success;

        GLuint program;
    };

    class OpenGLRenderDriverShaderCompiler {
    private:
        OpenGLRenderDriverShaderCompiler() = delete;
        ~OpenGLRenderDriverShaderCompiler() = delete;
    public:
        static OpenGLRenderDriverShaderCompilationResult Compile(const char *vertex_source, const char *fragment_source);
    };

}