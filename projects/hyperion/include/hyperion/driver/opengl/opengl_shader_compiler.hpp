#pragma once

#include <glad/glad.h>

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    struct OpenGLShaderCompilationResult {
        bool success = false;
        GLuint program;
    };

    class OpenGLShaderCompiler {
    public:
        static void Init();

        static OpenGLShaderCompilationResult Compile(const String &source);
        static String GetShaderModuleFromName(const String &name);
    private:
        inline static Map<String, String> s_modules;
    private:
        OpenGLShaderCompiler() = delete;
        ~OpenGLShaderCompiler() = delete;
    };

}