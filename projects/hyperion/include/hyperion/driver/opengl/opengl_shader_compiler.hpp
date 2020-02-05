#pragma once

#include "hyperion/rendering/shader.hpp"

namespace Hyperion::Rendering {
    
    class OpenGLShaderCompiler {
    private:
        inline static u32 s_fallback_shader;
        inline static Map<String, String> s_modules;
    public:
        static u32 Compile(const String &source);
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