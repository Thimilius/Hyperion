//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/shader/shader_pre_processor.hpp"

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
    bool ShaderPreProcessor::HandleDirective(ShaderPreProcessResult &result) {
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
                result.data.attributes.light_mode = shader_light_mode;
                m_property_light_mode_set = true;
            }

            m_source = m_source.replace(directive_start_position, directive_full_length, "");
            m_position = directive_start_position;
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
            return ShaderStageFlags::Unknown;
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

}