#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_shader.hpp"

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"

#include <glad/glad.h>

namespace Hyperion::Rendering {

    OpenGLShader::OpenGLShader(const String &name, const String &source) : Shader(name) {
        Recompile(source);
    }
    
    OpenGLShader::OpenGLShader(const String &name, const String &vertex_source, const String &fragment_source) : Shader(name) {
        Recompile(vertex_source, fragment_source);
    }

    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(m_shader_program_id);
    }

    void OpenGLShader::Bind() const {
        glUseProgram(m_shader_program_id);
    }

    void OpenGLShader::Unbind() const {
        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const String &name, u32 value) {
        glUniform1i(TryGetUniformLocation(name), value);
    }

    void OpenGLShader::SetFloat(const String &name, f32 value) {
        glUniform1f(TryGetUniformLocation(name), value);
    }

    void OpenGLShader::SetFloat2(const String &name, const Vec2 &value) {
        glUniform2f(TryGetUniformLocation(name), value.x, value.y);
    }

    void OpenGLShader::SetFloat3(const String &name, const Vec3 &value) {
        glUniform3f(TryGetUniformLocation(name), value.x, value.y, value.z);
    }

    void OpenGLShader::SetFloat4(const String &name, const Vec4 &value) {
        glUniform4f(TryGetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetMat4(const String &name, const Mat4 &matrix) {
        glUniformMatrix4fv(TryGetUniformLocation(name), 1, GL_FALSE, matrix.elements);
    }

    void OpenGLShader::Recompile(const String &source) {
        OpenGLShaderPreProcessResult result = OpenGLShaderCompiler::PreProcess(source);
        if (result.status == OpenGLShaderPreProcessStatus::Success) {
            Compile(result.sources);
        } else {
            CompileFallbackShader();
        }
    }

    void OpenGLShader::Recompile(const String &vertex_source, const String &fragment_source) {
        Map<ShaderType, String> sources(2);
        sources[ShaderType::Vertex] = vertex_source;
        sources[ShaderType::Fragment] = fragment_source;

        Compile(sources);
    }

    void OpenGLShader::Compile(const Map<ShaderType, String> &sources) {
        // Clear uniforms
        m_uniforms.clear();

        // Compile shaders
        Vector<u32> shaders;
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

                delete[] info_log;

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

                delete[] info_log;

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

    void OpenGLShader::CompileFallbackShader() {
        Map<ShaderType, String> sources;
        sources[ShaderType::Vertex] = R"(
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
        sources[ShaderType::Fragment] = R"(
            #version 410 core

            out vec4 o_color;

            void main() {
                o_color = vec4(0, 1, 1, 1);
            }
        )";
        Compile(sources);
    }

    s32 OpenGLShader::TryGetUniformLocation(const String &name) {
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

    u32 OpenGLShader::GetGLShaderType(ShaderType type) {
        switch (type) {
            case ShaderType::Vertex: return GL_VERTEX_SHADER;
            case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}