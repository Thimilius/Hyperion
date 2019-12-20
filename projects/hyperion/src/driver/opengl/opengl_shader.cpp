#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_shader.hpp"

namespace Hyperion::Rendering {

    COpenGLShader::COpenGLShader(const TString &name, const TString &source) : CShader(name) {
        Recompile(source);
    }
    
    COpenGLShader::COpenGLShader(const TString &name, const TString &vertex_source, const TString &fragment_source) : CShader(name) {
        Recompile(vertex_source, fragment_source);
    }

    COpenGLShader::~COpenGLShader() {
        glDeleteProgram(m_shader_program_id);
    }

    void COpenGLShader::Bind() const {
        glUseProgram(m_shader_program_id);
    }

    void COpenGLShader::Unbind() const {
        glUseProgram(0);
    }

    void COpenGLShader::SetInt(const TString &name, int value) {
        glUniform1i(TryGetUniformLocation(name), value);
    }

    void COpenGLShader::SetFloat(const TString &name, float value) {
        glUniform1f(TryGetUniformLocation(name), value);
    }

    void COpenGLShader::SetFloat2(const TString &name, const Math::SVec2 &value) {
        glUniform2f(TryGetUniformLocation(name), value.x, value.y);
    }

    void COpenGLShader::SetFloat3(const TString &name, const Math::SVec3 &value) {
        glUniform3f(TryGetUniformLocation(name), value.x, value.y, value.z);
    }

    void COpenGLShader::SetFloat4(const TString &name, const Math::SVec4 &value) {
        glUniform4f(TryGetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void COpenGLShader::SetMat4(const TString &name, const Math::SMat4 &matrix) {
        glUniformMatrix4fv(TryGetUniformLocation(name), 1, GL_FALSE, matrix.elements);
    }

    void COpenGLShader::Recompile(const TString &source) {
        Compile(PreProcess(source));
    }

    void COpenGLShader::Recompile(const TString &vertex_source, const TString &fragment_source) {
        TMap<EShaderType, TString> sources(2);
        sources[EShaderType::Vertex] = vertex_source;
        sources[EShaderType::Fragment] = fragment_source;

        Compile(sources);
    }

    TMap<EShaderType, TString> COpenGLShader::PreProcess(const TString &source) {
        TMap<EShaderType, TString> sources;

        const auto type_token = "#type";
        u64 type_token_length = strlen(type_token);
        u64 position = source.find(type_token, 0);

        while (position != TString::npos) {
            u64 end_of_line = source.find_first_of("\r\n", position);
            HYP_ASSERT_MESSAGE(end_of_line != TString::npos, "Shader syntax error!");
            u64 begin = position + type_token_length + 1;
            TString type = source.substr(begin, end_of_line - begin);
            HYP_ASSERT_MESSAGE(type == "vertex" || type == "fragment", "Invalid shader type specifier!");

            u64 next_line_position = source.find_first_not_of("\r\n", end_of_line);
            position = source.find(type_token, next_line_position);

            u64 end = position - (next_line_position == TString::npos ? source.size() - 1 : next_line_position);
            sources[ShaderTypeFromString(type)] = source.substr(next_line_position, end);
        }

        return sources;
    }

    void COpenGLShader::Compile(TMap<EShaderType, TString> sources) {
        // Clear uniforms
        m_uniforms.clear();

        // Compile shaders
        TVector<u32> shaders;
        for (auto pair : sources) {
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

                delete info_log;

                CompileFallbackShader();

                return;
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
            glGetProgramiv(program, GL_LINK_STATUS, (int *)& is_linked);
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

                delete info_log;

                CompileFallbackShader();

                return;
            }
        }

        m_shader_program_id = program;

        for (u32 shader : shaders) {
            glDetachShader(program, shader);
            glDeleteShader(shader);
        }
    }

    void COpenGLShader::CompileFallbackShader() {
        TMap<EShaderType, TString> sources;
        sources[EShaderType::Vertex] = R"(
            #version 410 core

            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec3 a_normal;
            layout(location = 2) in vec2 a_uv;

            uniform struct Transform {
	            mat4 model;
	            mat4 view;
	            mat4 projection;
            } u_transform;

            vec4 obj_to_clip_space(vec3 position) {
	            return u_transform.projection * u_transform.view * u_transform.model * vec4(position, 1.0f);
            }

            void main() {
                gl_Position = obj_to_clip_space(a_position);
            }
        )";
        sources[EShaderType::Fragment] = R"(
            #version 410 core

            out vec4 o_color;

            void main() {
	            o_color = vec4(0, 1, 1, 1);
            }
        )";
        Compile(sources);
    }

    u32 COpenGLShader::GetGLShaderType(EShaderType type) {
        switch (type) {
            case Hyperion::Rendering::EShaderType::Vertex: return GL_VERTEX_SHADER;
            case Hyperion::Rendering::EShaderType::Fragment: return GL_FRAGMENT_SHADER;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    s32 COpenGLShader::TryGetUniformLocation(const TString &name) {
        auto loc = m_uniforms.find(name);
        if (loc == m_uniforms.end()) {
            s32 location = glGetUniformLocation(m_shader_program_id, name.c_str());
            if (location < 0) {
                HYP_LOG_ERROR("OpenGL", "Failed to get location for uniform: {}", name);
            }
            m_uniforms[name] = location;
            return location;
        }
        return loc->second;
    }

}