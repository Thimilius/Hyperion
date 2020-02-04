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

    class OpenGLShaderPreProcessor {
    private:
        String m_source;
        u64 m_position;

        ShaderType m_current_shader_type = ShaderType::Unknown;
        u64 m_current_shader_type_directive_end;
    public:
        OpenGLShaderPreProcessor(const String &source);

        OpenGLShaderPreProcessResult PreProcess();
    private:
        bool HandleDirective(Map<ShaderType, String> &sources);
        void EndShaderType(Map<ShaderType, String> &sources, u64 end_position);

        char Advance();
        String AdvanceUntilEndOfLine();
        char Peek();
        char PeekNext();

        void SkipAlpha();
        void SkipWhitespace();

        bool IsAtEnd();
        bool IsAlpha(char c);
        bool IsWhitespace(char c);

        static ShaderType GetShaderTypeFromString(const String &string);
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

        static String GetShaderModuleFromName(const String &name);
        static u32 GetGLShaderType(ShaderType type);

        friend class OpenGLShaderPreProcessor;
        friend class OpenGLRenderDriver;
    };

}