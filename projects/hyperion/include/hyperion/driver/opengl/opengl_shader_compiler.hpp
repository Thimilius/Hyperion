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
        inline static Map<String, String> s_modules;
    public:
        static OpenGLShaderPreProcessResult PreProcess(const String &source);
    private:
        OpenGLShaderCompiler() = delete;
        ~OpenGLShaderCompiler() = delete;

        static void Init();

        static ShaderType ShaderTypeFromString(const String &string);
        static String ShaderModuleFromString(const String &string);

        friend class OpenGLRenderDriver;
    };

}