#pragma once

#include "hyperion/rendering/shader.hpp"

namespace Hyperion::Rendering {
    
    struct OpenGLShaderCompilerResult {
        ShaderProperties properties;
        u32 id;
    };

    class OpenGLShaderCompiler {
    private:
        inline static OpenGLShaderCompilerResult s_fallback_shader;
        inline static Map<String, String> s_modules;
    public:
        static OpenGLShaderCompilerResult Compile(const String &source);
    private:
        OpenGLShaderCompiler() = delete;
        ~OpenGLShaderCompiler() = delete;

        static void Init();

        static String GetShaderModuleFromName(const String &name);
        static u32 GetGLShaderType(ShaderType type);

        friend class OpenGLShaderPreProcessor;
        friend class OpenGLRenderDriver;
    };

}