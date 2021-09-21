#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/types/render_types_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct ShaderPreProcessResult {
        bool success;

        ShaderData data;
    };

    class ShaderPreProcessor {
    public:
        ShaderPreProcessor(const String &source);
    public:
        ShaderPreProcessResult PreProcess();
    private:
        bool HandleDirective(ShaderPreProcessResult &result);
        void EndShaderStage(ShaderPreProcessResult &result, uint64 end_position);
        void AddDefine(const String &define);

        char Advance();
        String AdvanceUntilWhitespaceOrEndOfLine();
        char Peek();
        char PeekNext();

        void SkipAlphaNumeric();
        void SkipBlankspace();
        void SkipToNextLine();

        bool IsAtEnd();
        bool IsAlpha(char c);
        bool IsNumeric(char c);
        bool IsWhitespace(char c);

        bool IsDirective(const char *directive, const char *start);
    private:
        static ShaderProperty CreateShaderProperty(const String &name, ShaderPropertyType type);

        static ShaderStageFlags GetShaderStageFromString(const String &string);
        static ShaderLightMode GetShaderLightModeFromString(const String &string);
        static ShaderPropertyType GetShaderPropertyType(const String &string);
    private:
        String m_source;
        uint64 m_position;

        ShaderStageFlags m_current_shader_stage = ShaderStageFlags::Unknown;
        uint64 m_current_shader_type_directive_end;

        bool m_property_light_mode_set = false;
    };

}