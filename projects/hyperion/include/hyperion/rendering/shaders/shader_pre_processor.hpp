#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/attributes/shader_attributes.hpp"
#include "hyperion/rendering/shaders/shader_modules.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct ShaderPreProcessResult {
        bool success;

        ShaderAttributes attributes;
        ShaderStageFlags stage_flags;
        Map<ShaderStageFlags, String> sources;
    };

    class ShaderPreProcessor {
    public:
        ShaderPreProcessor(const String &source);
    public:
        ShaderPreProcessResult PreProcess();
    private:
        bool HandleDirective(ShaderStageFlags &stage_flags, Map<ShaderStageFlags, String> &sources, ShaderAttributes &attributes);
        void EndShaderStage(Map<ShaderStageFlags, String> &sources, uint64 end_position);

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
    private:
        static ShaderStageFlags GetShaderStageFromString(const String &string);
        static ShaderLightMode GetShaderLightModeFromString(const String &string);
        static ShaderModuleType GetShaderModuleTypeFromString(const String &string);
    private:
        String m_source;
        uint64 m_position;

        ShaderStageFlags m_current_shader_type = ShaderStageFlags::None;
        uint64 m_current_shader_type_directive_end;

        bool m_property_light_mode_set = false;
    };

}