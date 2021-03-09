//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/shaders/shader_pre_processor.hpp"

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

        Map<ShaderStageFlags, String> sources;
        ShaderStageFlags stage_flags = ShaderStageFlags::None;
        ShaderAttributes attributes = { };
        String source = m_source;

        while (!IsAtEnd()) {
            // Start of file counts as "new line".
            if (Peek() == '\n' || m_position == 0) {
                if (Peek() == '\n') {
                    Advance();
                }
                SkipBlankspace();

                if (Peek() == '#') {
                    if (!HandleDirective(stage_flags, sources, attributes)) {
                        return result;
                    }
                }
            } else {
                Advance();
            }
        }
        EndShaderStage(sources, m_position);

        // Check that we found both a vertex and fragment source.
        if ((stage_flags & ShaderStageFlags::Vertex) != ShaderStageFlags::Vertex) {
            HYP_LOG_ERROR("OpenGL", "Shader does not contain a vertex shader!");
            return result;
        } else if ((stage_flags & ShaderStageFlags::Fragment) != ShaderStageFlags::Fragment) {
            HYP_LOG_ERROR("OpenGL", "Shader does not contain a fragment shader!");
            return result;
        }

        result.success = true;
        result.attributes = std::move(attributes);
        result.stage_flags = stage_flags;
        result.sources = std::move(sources);

        return result;
    }

    //--------------------------------------------------------------
    bool ShaderPreProcessor::HandleDirective(ShaderStageFlags &stage_flags, Map<ShaderStageFlags, String> &sources, ShaderAttributes &attributes) {
        uint64 directive_start_position = m_position;
        Advance();

        char *directive_start = m_source.data() + m_position;
        SkipAlphaNumeric();

        if (IsDirective("version", directive_start)) {
            SkipToNextLine();

            // Here is the point where we can dynamically add defines into the shader.
#if HYP_EDITOR
            AddDefine("HYP_EDITOR");
#endif
        } else if (IsDirective("type", directive_start)) {
            EndShaderStage(sources, directive_start_position);

            // A new shader type starts.
            {
                SkipBlankspace();
                String type_string = AdvanceUntilWhitespaceOrEndOfLine();

                ShaderStageFlags shader_stage = GetShaderStageFromString(type_string);
                if (shader_stage == ShaderStageFlags::None) {
                    HYP_LOG_ERROR("OpenGL", "Invalid shader stage specifier: '{}'!", type_string);
                    return false;
                }

                stage_flags |= shader_stage;
                m_current_shader_type = shader_stage;
                m_current_shader_type_directive_end = m_position;
            }
        } else if (IsDirective("import", directive_start)) {
            SkipBlankspace();
            String import_string = AdvanceUntilWhitespaceOrEndOfLine();
            uint64 directive_full_length = m_position - directive_start_position;

            ShaderModuleType shader_module_type = GetShaderModuleTypeFromString(import_string);
            if (shader_module_type == ShaderModuleType::Unknown) {
                HYP_LOG_ERROR("OpenGL", "Invalid shader import module: '{}'!", import_string);
                return false;
            }

            String shader_module = ShaderModules::GetModule(shader_module_type);
            m_source = m_source.replace(directive_start_position, directive_full_length, shader_module);
            m_position = directive_start_position + shader_module.size();
        } else if (IsDirective("light_mode", directive_start)) {
            SkipBlankspace();
            String light_mode_string = AdvanceUntilWhitespaceOrEndOfLine();
            uint64 directive_full_length = m_position - directive_start_position;

            ShaderLightMode shader_light_mode = GetShaderLightModeFromString(light_mode_string);
            if (shader_light_mode == ShaderLightMode::Unknown) {
                HYP_LOG_ERROR("OpenGL", "Invalid shader light mode specifier: '{}'!", light_mode_string);
                return false;
            }

            if (m_property_light_mode_set) {
                HYP_LOG_WARN("OpenGL", "The shader light mode was already set!");
            } else {
                attributes.light_mode = shader_light_mode;
                m_property_light_mode_set = true;
            }

            m_source = m_source.replace(directive_start_position, directive_full_length, "");
            m_position = directive_start_position;
        }

        return true;
    }

    //--------------------------------------------------------------
    void ShaderPreProcessor::EndShaderStage(Map<ShaderStageFlags, String> &sources, uint64 end_position) {
        if (m_current_shader_type != ShaderStageFlags::None) {
            uint64 source_length = end_position - m_current_shader_type_directive_end;
            String source = m_source.substr(m_current_shader_type_directive_end, source_length);
            sources[m_current_shader_type] = source;
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
    bool ShaderPreProcessor::IsAtEnd() {
        return m_position >= m_source.size();
    }

    //--------------------------------------------------------------
    bool ShaderPreProcessor::IsAlpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';;
    }

    //--------------------------------------------------------------
    bool ShaderPreProcessor::IsNumeric(char c) {
        return c >= '0' && c <= '9';
    }

    //--------------------------------------------------------------
    bool ShaderPreProcessor::IsWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\r';
    }

    //--------------------------------------------------------------
    bool ShaderPreProcessor::IsDirective(const char *directive, const char *start) {
        return std::memcmp(directive, (void *)start, std::strlen(directive)) == 0;
    }

    //--------------------------------------------------------------
    ShaderStageFlags ShaderPreProcessor::GetShaderStageFromString(const String &string) {
        if (string == "vertex") {
            return ShaderStageFlags::Vertex;
        } else if (string == "fragment") {
            return ShaderStageFlags::Fragment;
        } else {
            return ShaderStageFlags::None;
        }
    }

    //--------------------------------------------------------------
    ShaderLightMode ShaderPreProcessor::GetShaderLightModeFromString(const String &string) {
        if (string == "none") {
            return ShaderLightMode::None;
        } else if (string == "forward") {
            return ShaderLightMode::Forward;
        } else {
            return ShaderLightMode::Unknown;
        }
    }

    //--------------------------------------------------------------
    ShaderModuleType ShaderPreProcessor::GetShaderModuleTypeFromString(const String &string) {
        return ShaderModuleType::Unknown;
    }

}