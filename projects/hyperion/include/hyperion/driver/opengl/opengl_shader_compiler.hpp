#pragma once

#include "hyperion/rendering/shader.hpp"

namespace Hyperion::Rendering {
    class OpenGLRenderDriver;
    class OpenGLShaderPreProcessor;
}

namespace Hyperion::Rendering {
    
    struct OpenGLShaderCompilerResult {
        ShaderAttributes properties;
        u32 id;
    };

    class OpenGLShaderCompiler {
    public:
        static OpenGLShaderCompilerResult Compile(const String &source);
    private:
        OpenGLShaderCompiler() = delete;
        ~OpenGLShaderCompiler() = delete;

        static void Init();

        static String GetShaderModuleFromName(const String &name);
        static u32 GetGLShaderType(ShaderType type);
    private:
        inline static OpenGLShaderCompilerResult s_fallback_shader;
        inline static Map<String, String> s_modules;
    private:
        friend class Hyperion::Rendering::OpenGLRenderDriver;
        friend class Hyperion::Rendering::OpenGLShaderPreProcessor;
    };

}