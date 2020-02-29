#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_shader_pre_processor.hpp"

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"

namespace Hyperion::Rendering {

    OpenGLShaderPreProcessor::OpenGLShaderPreProcessor(const String &source) {
        m_source = source;
        m_position = 0;
    }

    OpenGLShaderPreProcessResult OpenGLShaderPreProcessor::PreProcess() {
        OpenGLShaderPreProcessResult result;
        result.status = OpenGLShaderPreProcessStatus::Failed;

        Map<ShaderType, String> sources;
        String source = m_source;

        while (!IsAtEnd()) {
            // Start of file counts as "new line"
            if (Peek() == '\n' || m_position == 0) {
                if (Peek() == '\n') {
                    Advance();
                }
                SkipBlankspace();

                if (Peek() == '#') {
                    if (!HandleDirective(sources)) {
                        return result;
                    }
                }
            } else {
                Advance();
            }
        }
        EndShaderType(sources, m_position);

        // Check that we found both a vertex and fragment source
        if (sources.find(ShaderType::Vertex) == sources.end()) {
            HYP_LOG_ERROR("OpenGL", "Shader does not contain a vertex shader!");
            return result;
        } else if (sources.find(ShaderType::Fragment) == sources.end()) {
            HYP_LOG_ERROR("OpenGL", "Shader does not contain a fragment shader!");
            return result;
        }

        result.status = OpenGLShaderPreProcessStatus::Success;
        result.sources = std::move(sources);

        return result;
    }

    bool OpenGLShaderPreProcessor::HandleDirective(Map<ShaderType, String> &sources) {
        u64 directive_start_position = m_position;
        Advance();

        char *directive_type_start = m_source.data() + m_position;
        SkipAlpha();

        if (std::memcmp("type", (void *)directive_type_start, 4) == 0) {
            EndShaderType(sources, directive_start_position);

            // A new shader type starts
            {
                SkipBlankspace();
                String type_string = AdvanceUntilEndOfLine();

                ShaderType shader_type = GetShaderTypeFromString(type_string);
                if (shader_type == ShaderType::Unknown) {
                    HYP_LOG_ERROR("OpenGL", "Invalid shader type specifier: '{}'!", type_string);
                    return false;
                }

                m_current_shader_type = shader_type;
                m_current_shader_type_directive_end = m_position;
            }
        } else if (std::memcmp("import", (void *)directive_type_start, 6) == 0) {
            SkipBlankspace();
            String import_string = AdvanceUntilEndOfLine();
            u64 directive_full_length = m_position - directive_start_position;

            String shader_module = OpenGLShaderCompiler::GetShaderModuleFromName(import_string);
            if (shader_module.empty()) {
                HYP_LOG_ERROR("OpenGL", "Invalid shader import module: '{}'!", import_string);
                return false;
            }

            m_source = m_source.replace(directive_start_position, directive_full_length, shader_module);
            m_position = directive_start_position + shader_module.size();
        }

        return true;
    }

    void OpenGLShaderPreProcessor::EndShaderType(Map<ShaderType, String> &sources, u64 end_position) {
        if (m_current_shader_type != ShaderType::Unknown) {
            u64 source_length = end_position - m_current_shader_type_directive_end;
            String source = m_source.substr(m_current_shader_type_directive_end, source_length);
            sources[m_current_shader_type] = source;
        }
    }

    char OpenGLShaderPreProcessor::Advance() {
        m_position++;
        return m_source[m_position - 1];
    }

    String OpenGLShaderPreProcessor::AdvanceUntilEndOfLine() {
        u64 import_start_position = m_position;
        SkipAlpha();
        u64 import_end_position = m_position;
        u64 import_length = import_end_position - import_start_position;

        return m_source.substr(import_start_position, import_length);
    }

    char OpenGLShaderPreProcessor::Peek() {
        return m_source[m_position];
    }

    char OpenGLShaderPreProcessor::PeekNext() {
        if (IsAtEnd()) {
            return '\0';
        }
        return m_source[m_position];
    }

    void OpenGLShaderPreProcessor::SkipAlpha() {
        while (IsAlpha(Peek()) && !IsAtEnd()) {
            Advance();
        }
    }

    void OpenGLShaderPreProcessor::SkipBlankspace() {
        while (IsWhitespace(Peek()) && !IsAtEnd()) {
            Advance();
        }
    }

    bool OpenGLShaderPreProcessor::IsAtEnd() {
        return m_position >= m_source.size();
    }

    bool OpenGLShaderPreProcessor::IsAlpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';;
    }

    bool OpenGLShaderPreProcessor::IsWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\r';
    }

    ShaderType OpenGLShaderPreProcessor::GetShaderTypeFromString(const String &string) {
        if (string == "vertex") {
            return ShaderType::Vertex;
        } else if (string == "fragment") {
            return ShaderType::Fragment;
        } else {
            return ShaderType::Unknown;
        }
    }

}