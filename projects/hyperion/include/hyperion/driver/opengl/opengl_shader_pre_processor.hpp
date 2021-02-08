#pragma once

#include "hyperion/rendering/attributes/shader_attributes.hpp"

namespace Hyperion::Rendering {

    struct OpenGLShaderPreProcessResult {
        bool success;
        ShaderAttributes properties;
        Map<ShaderType, String> sources;
    };

    class OpenGLShaderPreProcessor {
    public:
        OpenGLShaderPreProcessor(const String &source);
    public:
        OpenGLShaderPreProcessResult PreProcess();
    private:
        bool HandleDirective(Map<ShaderType, String> &sources, ShaderAttributes &properties);
        void EndShaderType(Map<ShaderType, String> &sources, uint64 end_position);

        char Advance();
        String AdvanceUntilEndOfLine();
        char Peek();
        char PeekNext();

        void SkipAlphaNumeric();
        void SkipBlankspace();

        bool IsAtEnd();
        bool IsAlpha(char c);
        bool IsNumeric(char c);
        bool IsWhitespace(char c);

        bool IsDirective(const char *directive, const char *start);

        static ShaderType GetShaderTypeFromString(const String &string);
        static ShaderLightMode GetShaderLightModeFromString(const String &string);
    private:
        String m_source;
        uint64 m_position;

        ShaderType m_current_shader_type = ShaderType::None;
        uint64 m_current_shader_type_directive_end;

        bool m_property_light_mode_set = false;
    };

}