#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"

#include "hyperion/assets/asset_library.hpp"
#include "hyperion/core/io/file_utilities.hpp"

#include <glad/glad.h>
#include <sstream>

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

        SkipWhitespace();
        if (Peek() == '#') {
            if (!HandleDirective(sources)) {
                return result;
            }
        }
        while (!IsAtEnd()) {
            char c = Peek();
            if (c == '\n') {
                Advance();

                SkipWhitespace();

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
                SkipWhitespace();
                String type_string = AdvanceUntilEndOfLine();
                u64 directive_full_length = m_position - directive_start_position;

                ShaderType shader_type = GetShaderTypeFromString(type_string);
                if (shader_type == ShaderType::Unknown) {
                    HYP_LOG_ERROR("OpenGL", "Invalid shader type specifier: '{}'!", type_string);
                    return false;
                }

                m_current_shader_type = shader_type;
                m_current_shader_type_directive_end = m_position;
            }
        } else if (std::memcmp("import", (void *)directive_type_start, 6) == 0) {
            SkipWhitespace();
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

    void OpenGLShaderPreProcessor::SkipWhitespace() {
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
        return c == ' ' || c == '\r' || c == '\r';
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

    u32 OpenGLShaderCompiler::Compile(const String &source) {
        OpenGLShaderPreProcessor pre_processor(source);
        OpenGLShaderPreProcessResult pre_process_result = pre_processor.PreProcess();
        if (pre_process_result.status == OpenGLShaderPreProcessStatus::Failed) {
            return s_fallback_shader;
        }

        // Compile shaders
        Vector<u32> shaders;
        for (auto pair : pre_process_result.sources) {
            u32 shader = glCreateShader(GetGLShaderType(pair.first));

            const GLchar *source = pair.second.c_str();
            glShaderSource(shader, 1, &source, 0);

            glCompileShader(shader);

            GLint is_compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
            if (is_compiled == GL_FALSE) {
                GLint log_length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

                GLchar *info_log = new GLchar[log_length];
                glGetShaderInfoLog(shader, log_length, &log_length, info_log);

                for (u32 id : shaders) {
                    glDeleteShader(id);
                }
                glDeleteShader(shader);

                HYP_LOG_ERROR("OpenGL", "Shader compilation error:\n{}", info_log);

                delete[] info_log;

                return s_fallback_shader;
            }

            shaders.push_back(shader);
        }

        // Link program
        u32 program = glCreateProgram();
        {
            for (u32 id : shaders) {
                glAttachShader(program, id);
            }
            glLinkProgram(program);

            GLint is_linked = 0;
            glGetProgramiv(program, GL_LINK_STATUS, (int *)&is_linked);
            if (is_linked == GL_FALSE) {
                GLint log_length = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

                GLchar *info_log = new GLchar[log_length];
                glGetProgramInfoLog(program, log_length, &log_length, info_log);

                for (u32 id : shaders) {
                    glDeleteShader(id);
                }
                glDeleteProgram(program);

                HYP_LOG_ERROR("OpenGL", "Shader linking error:\n{}", info_log);

                delete[] info_log;

                return s_fallback_shader;
            }
        }

        for (u32 shader : shaders) {
            glDetachShader(program, shader);
            glDeleteShader(shader);
        }

        return program;
    }

    void OpenGLShaderCompiler::Init() {
        // FIXME: There should probably a better and more global way of getting configuration stuff
        const String &shader_path = AssetLibrary::GetSettings().shader_path;
        for (auto &entry : std::filesystem::directory_iterator(shader_path + "/modules")) {
            auto &path = entry.path();
            if (path.extension() == ".glsl_module") {
                String filename = path.filename().u8string();
                String module_name = filename.substr(0, filename.find_last_of('.'));
                s_modules[module_name] = FileUtilities::ReadAllText(path.u8string());
            }
        }

        s_fallback_shader = Compile(FileUtilities::ReadAllText(shader_path + "/internal/standard_fallback.glsl"));
    }

    String OpenGLShaderCompiler::GetShaderModuleFromName(const String &name) {
        if (s_modules.find(name) == s_modules.end()) {
            return String();
        }

        return s_modules[name];
    }

    u32 OpenGLShaderCompiler::GetGLShaderType(ShaderType type) {
        switch (type) {
            case ShaderType::Vertex: return GL_VERTEX_SHADER;
            case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}