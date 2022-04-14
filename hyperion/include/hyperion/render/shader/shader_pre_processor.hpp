//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/types/render_types_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  struct ShaderPreProcessResult {
    bool8 success;

    ShaderData data;
  };

  class ShaderPreProcessor {
  public:
    ShaderPreProcessor(const String &source);
  public:
    ShaderPreProcessResult PreProcess();
  private:
    bool8 HandleDirective(ShaderPreProcessResult &result);
    void EndShaderStage(ShaderPreProcessResult &result, uint64 end_position);
    void AddDefine(const String &define);

    char Advance();
    String AdvanceUntilWhitespaceOrEndOfLine();
    char Peek();
    char PeekNext();

    void SkipAlphaNumeric();
    void SkipBlankspace();
    void SkipToNextLine();

    bool8 IsAtEnd();
    bool8 IsAlpha(char c);
    bool8 IsNumeric(char c);
    bool8 IsWhitespace(char c);

    bool8 IsDirective(const char *directive, const char *start);
  private:
    static ShaderProperty CreateShaderProperty(const String &name, ShaderPropertyType type);

    static ShaderStageFlags GetShaderStageFromString(const String &string);
    static ShaderRenderOrder GetShaderRenderOrderFromString(const String &string);
    static ShaderCullingMode GetShaderCullingModeFromString(const String &string);
    static ShaderBlendingMode GetShaderBlendingModeFromString(const String &string);
    static ShaderZWrite GetShaderZWriteFromString(const String &string);
    static ShaderPropertyType GetShaderPropertyType(const String &string);
  private:
    String m_source;
    uint64 m_position;

    ShaderStageFlags m_current_shader_stage = ShaderStageFlags::Unknown;
    uint64 m_current_shader_type_directive_end;
  };

}
