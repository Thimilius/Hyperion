//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/shader/shader_pre_processor.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  ShaderPreProcessor::ShaderPreProcessor(const String &source) {
    m_source = source;
    m_position = 0;
  }

  //--------------------------------------------------------------
  ShaderPreProcessResult ShaderPreProcessor::PreProcess() {
    ShaderPreProcessResult result = { };
    result.success = false;

    String source = m_source;

    while (!IsAtEnd()) {
      // Start of file counts as "new line".
      if (Peek() == '\n' || m_position == 0) {
        if (Peek() == '\n') {
          Advance();
        }
        SkipBlankspace();

        if (Peek() == '#') {
          if (!HandleDirective(result)) {
            return result;
          }
        }
      } else {
        Advance();
      }
    }
    EndShaderStage(result, m_position);

    // Check that we found both a vertex and fragment source.
    if ((result.data.stage_flags & ShaderStageFlags::Vertex) != ShaderStageFlags::Vertex) {
      HYP_LOG_ERROR("OpenGL", "Shader does not contain a vertex shader!");
      return result;
    } else if ((result.data.stage_flags & ShaderStageFlags::Fragment) != ShaderStageFlags::Fragment) {
      HYP_LOG_ERROR("OpenGL", "Shader does not contain a pixel shader!");
      return result;
    }

    result.success = true;

    return result;
  }

  //--------------------------------------------------------------
  bool8 ShaderPreProcessor::HandleDirective(ShaderPreProcessResult &result) {
    uint64 directive_start_position = m_position;
    Advance();

    char *directive_start = m_source.data() + m_position;
    SkipAlphaNumeric();

    if (IsDirective("version", directive_start)) {
      SkipToNextLine();

      // Here is the point where we can dynamically add defines into the shader.
    } else if (IsDirective("type", directive_start)) {
      EndShaderStage(result, directive_start_position);

      // A new shader type starts.
      {
        SkipBlankspace();
        String type_string = AdvanceUntilWhitespaceOrEndOfLine();

        ShaderStageFlags shader_stage = GetShaderStageFromString(type_string);
        if (shader_stage == ShaderStageFlags::Unknown) {
          HYP_LOG_ERROR("OpenGL", "Invalid shader stage specifier: '{}'!", type_string);
          return false;
        }

        result.data.stage_flags |= shader_stage;
        m_current_shader_stage = shader_stage;
        m_current_shader_type_directive_end = m_position;
      }
    } else if (IsDirective("render_order", directive_start)) {
      SkipBlankspace();
      String render_order_string = AdvanceUntilWhitespaceOrEndOfLine();
      uint64 directive_full_length = m_position - directive_start_position;

      ShaderRenderOrder render_order_shader = GetShaderRenderOrderFromString(render_order_string);
      if (render_order_shader == ShaderRenderOrder::Unknown) {
        HYP_LOG_ERROR("OpenGL", "Invalid shader render order specifier: '{}'!", render_order_string);
        return false;
      }

      result.data.attributes.render_order = render_order_shader;

      m_source = m_source.replace(directive_start_position, directive_full_length, "");
      m_position = directive_start_position;
    } else if (IsDirective("culling_mode", directive_start)) {
      SkipBlankspace();
      String culling_mode_string = AdvanceUntilWhitespaceOrEndOfLine();
      uint64 directive_full_length = m_position - directive_start_position;

      ShaderCullingMode culling_mode_shader = GetShaderCullingModeFromString(culling_mode_string);
      if (culling_mode_shader == ShaderCullingMode::Unknown) {
        HYP_LOG_ERROR("OpenGL", "Invalid shader culling mode specifier: '{}'!", culling_mode_string);
        return false;
      }

      result.data.attributes.culling_mode = culling_mode_shader;

      m_source = m_source.replace(directive_start_position, directive_full_length, "");
      m_position = directive_start_position;
    } else if (IsDirective("blending_mode", directive_start)) {
      SkipBlankspace();
      String blending_mode_string = AdvanceUntilWhitespaceOrEndOfLine();
      uint64 directive_full_length = m_position - directive_start_position;

      ShaderBlendingMode blending_mode_shader = GetShaderBlendingModeFromString(blending_mode_string);
      if (blending_mode_shader == ShaderBlendingMode::Unknown) {
        HYP_LOG_ERROR("OpenGL", "Invalid shader blending mode specifier: '{}'!", blending_mode_string);
        return false;
      }

      result.data.attributes.blending_mode = blending_mode_shader;

      m_source = m_source.replace(directive_start_position, directive_full_length, "");
      m_position = directive_start_position;
    } else if (IsDirective("z_write", directive_start)) {
      SkipBlankspace();
      String z_write_string = AdvanceUntilWhitespaceOrEndOfLine();
      uint64 directive_full_length = m_position - directive_start_position;

      ShaderZWrite z_write_shader = GetShaderZWriteFromString(z_write_string);
      if (z_write_shader == ShaderZWrite::Unknown) {
        HYP_LOG_ERROR("OpenGL", "Invalid shader z write specifier: '{}'!", z_write_string);
        return false;
      }

      result.data.attributes.z_write = z_write_shader;

      m_source = m_source.replace(directive_start_position, directive_full_length, "");
      m_position = directive_start_position;
    } else if (IsDirective("property", directive_start)) {
      SkipBlankspace();

      String property_name = AdvanceUntilWhitespaceOrEndOfLine();
      if (property_name.empty()) {
        HYP_LOG_ERROR("OpenGL", "No name provided for property!");
        return false;
      }

      SkipBlankspace();
      String property_type_string = AdvanceUntilWhitespaceOrEndOfLine();
      ShaderPropertyType property_type = GetShaderPropertyType(property_type_string);
      if (property_type == ShaderPropertyType::Unknown) {
        HYP_LOG_ERROR("OpenGL", "Invalid property type specifier: '{}'!", property_type_string);
        return false;
      }

      result.data.properties.Add(CreateShaderProperty(property_name, property_type));
    }

    return true;
  }

  //--------------------------------------------------------------
  void ShaderPreProcessor::EndShaderStage(ShaderPreProcessResult &result, uint64 end_position) {
    if (m_current_shader_stage != ShaderStageFlags::Unknown) {
      uint64 source_length = end_position - m_current_shader_type_directive_end;
      String source = m_source.substr(m_current_shader_type_directive_end, source_length);

      switch (m_current_shader_stage) {
        case ShaderStageFlags::Vertex: result.data.vertex_source = source;
        case ShaderStageFlags::Fragment: result.data.fragment_source = source;
      }
    }
  }

  //--------------------------------------------------------------
  void ShaderPreProcessor::AddDefine(const String &define) {
    m_source.insert(m_position, String("\n#define ").append(define));
    // We know that we have a new line directly after the inserted string, so we do not need to add it ourselves.
  }

  //--------------------------------------------------------------
  char ShaderPreProcessor::Advance() {
    m_position++;
    return m_source[m_position - 1];
  }

  //--------------------------------------------------------------
  String ShaderPreProcessor::AdvanceUntilWhitespaceOrEndOfLine() {
    uint64 import_start_position = m_position;
    SkipAlphaNumeric();
    uint64 import_end_position = m_position;
    uint64 import_length = import_end_position - import_start_position;

    return m_source.substr(import_start_position, import_length);
  }

  //--------------------------------------------------------------
  char ShaderPreProcessor::Peek() {
    return m_source[m_position];
  }

  //--------------------------------------------------------------
  char ShaderPreProcessor::PeekNext() {
    if (IsAtEnd()) {
      return '\0';
    }
    return m_source[m_position];
  }

  //--------------------------------------------------------------
  void ShaderPreProcessor::SkipAlphaNumeric() {
    while ((IsAlpha(Peek()) || IsNumeric(Peek())) && !IsAtEnd()) {
      Advance();
    }
  }

  //--------------------------------------------------------------
  void ShaderPreProcessor::SkipBlankspace() {
    while (IsWhitespace(Peek()) && !IsAtEnd()) {
      Advance();
    }
  }

  //--------------------------------------------------------------
  void ShaderPreProcessor::SkipToNextLine() {
    while (!IsAtEnd()) {
      while (Peek() != '\n') {
        Advance();
      }
      return;
    }
  }

  //--------------------------------------------------------------
  bool8 ShaderPreProcessor::IsAtEnd() {
    return m_position >= m_source.size();
  }

  //--------------------------------------------------------------
  bool8 ShaderPreProcessor::IsAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';;
  }

  //--------------------------------------------------------------
  bool8 ShaderPreProcessor::IsNumeric(char c) {
    return c >= '0' && c <= '9';
  }

  //--------------------------------------------------------------
  bool8 ShaderPreProcessor::IsWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r';
  }

  //--------------------------------------------------------------
  bool8 ShaderPreProcessor::IsDirective(const char *directive, const char *start) {
    return std::memcmp(directive, (void *)start, std::strlen(directive)) == 0;
  }

  //--------------------------------------------------------------
  ShaderProperty ShaderPreProcessor::CreateShaderProperty(const String &name, ShaderPropertyType type) {
    ShaderProperty result;
    result.name = name;
    result.id = ShaderProperty::PropertyToId(name);
    result.type = type;

    // TODO: Add proper support for default values.
    switch (type) {
      case ShaderPropertyType::Float: result.storage.float32 = 0.0f;
        break;
      case ShaderPropertyType::Int: result.storage.int32 = 0;
        break;
      case ShaderPropertyType::Vector: result.storage.vector4 = Vector4();
        break;
      case ShaderPropertyType::Color: result.storage.color = Color::White();
        break;
      case ShaderPropertyType::Matrix: result.storage.matrix4x4 = Matrix4x4::Identity();
        break;
      case ShaderPropertyType::Texture: result.storage.texture.handle = AssetManager::GetTexture2DPrimitive(Texture2DPrimitive::White)->GetHandle();
        break;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        break;
    }

    return result;
  }

  //--------------------------------------------------------------
  ShaderStageFlags ShaderPreProcessor::GetShaderStageFromString(const String &string) {
    if (string == "vertex") {
      return ShaderStageFlags::Vertex;
    } else if (string == "fragment") {
      return ShaderStageFlags::Fragment;
    } else {
      return ShaderStageFlags::Unknown;
    }
  }

  //--------------------------------------------------------------
  ShaderRenderOrder ShaderPreProcessor::GetShaderRenderOrderFromString(const String &string) {
    if (string == "opaque") {
      return ShaderRenderOrder::Opaque;
    } else if (string == "transparent") {
      return ShaderRenderOrder::Transparent;
    } else {
      return ShaderRenderOrder::Unknown;
    }
  }

  //--------------------------------------------------------------
  ShaderCullingMode ShaderPreProcessor::GetShaderCullingModeFromString(const String &string) {
    if (string == "off") {
      return ShaderCullingMode::Off;
    } else if (string == "front") {
      return ShaderCullingMode::Front;
    } else if (string == "back") {
      return ShaderCullingMode::Back;
    } else {
      return ShaderCullingMode::Unknown;
    }
  }

  //--------------------------------------------------------------
  ShaderBlendingMode ShaderPreProcessor::GetShaderBlendingModeFromString(const String &string) {
    if (string == "on") {
      return ShaderBlendingMode::On;
    } else if (string == "off") {
      return ShaderBlendingMode::Off;
    } else {
      return ShaderBlendingMode::Unknown;
    }
  }

  //--------------------------------------------------------------
  ShaderZWrite ShaderPreProcessor::GetShaderZWriteFromString(const String &string) {
    if (string == "on") {
      return ShaderZWrite::On;
    } else if (string == "off") {
      return ShaderZWrite::Off;
    } else {
      return ShaderZWrite::Unknown;
    }
  }

  //--------------------------------------------------------------
  ShaderPropertyType ShaderPreProcessor::GetShaderPropertyType(const String &string) {
    if (string == "Float") {
      return ShaderPropertyType::Float;
    } else if (string == "Int") {
      return ShaderPropertyType::Int;
    } else if (string == "Vector") {
      return ShaderPropertyType::Vector;
    } else if (string == "Color") {
      return ShaderPropertyType::Color;
    } else if (string == "Matrix") {
      return ShaderPropertyType::Matrix;
    } else if (string == "Texture") {
      return ShaderPropertyType::Texture;
    } else {
      return ShaderPropertyType::Unknown;
    }
  }

}
