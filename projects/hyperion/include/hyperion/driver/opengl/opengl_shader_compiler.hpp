#pragma once

#include "hyperion/rendering/shader.hpp"

namespace Hyperion::Rendering {

    enum class OpenGLShaderPreProcessStatus {
        Failed,
        Success
    };

    struct OpenGLShaderPreProcessResult {
        OpenGLShaderPreProcessStatus status;
        Map<ShaderType, String> sources;
    };

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

        static OpenGLShaderPreProcessResult PreProcess(const String &source);

        static ShaderType ShaderTypeFromString(const String &string);
        static String ShaderModuleFromString(const String &string);
        static u32 GetGLShaderType(ShaderType type);

        friend class OpenGLRenderDriver;
    };

}