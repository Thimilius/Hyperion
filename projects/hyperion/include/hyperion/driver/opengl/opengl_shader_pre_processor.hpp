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
        void SkipBlankspace();

        bool IsAtEnd();
        bool IsAlpha(char c);
        bool IsWhitespace(char c);

        static ShaderType GetShaderTypeFromString(const String &string);
    };

}