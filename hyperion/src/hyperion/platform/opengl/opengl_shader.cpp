#include "hyppch.hpp"

#include "opengl_shader.hpp"

namespace Hyperion::Rendering {

    COpenGLShader::COpenGLShader(const TString &vertex_source, const TString &fragment_source) {
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        {
            const GLchar *source = vertex_source.c_str();
            glShaderSource(vertex_shader, 1, &source, 0);

            glCompileShader(vertex_shader);

            GLint is_compiled = 0;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
            if (is_compiled == GL_FALSE) {
                GLint log_length = 0;
                glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);

                GLchar *info_log = new GLchar[log_length];
                glGetShaderInfoLog(vertex_shader, log_length, &log_length, info_log);

                glDeleteShader(vertex_shader);

                HYP_CORE_ERROR("Vertex shader compilation error:\n{}", info_log);
                HYP_ASSERT_MESSAGE(false, "Vertex shader compilation failure!");

                delete info_log;

                return;
            }
        }

        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        {
            const GLchar *source = fragment_source.c_str();
            glShaderSource(fragment_shader, 1, &source, 0);

            glCompileShader(fragment_shader);

            GLint is_compiled = 0;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
            if (is_compiled == GL_FALSE) {
                GLint log_length = 0;
                glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);

                GLchar *info_log = new GLchar[log_length];
                glGetShaderInfoLog(vertex_shader, log_length, &log_length, info_log);

                glDeleteShader(fragment_shader);
                glDeleteShader(vertex_shader);

                HYP_CORE_ERROR("Fragment shader compilation error:\n{}", info_log);
                HYP_ASSERT_MESSAGE(false, "Fragment shader compilation failure!");

                delete info_log;

                return;
            }
        }

        m_program_id = glCreateProgram();
        {
            glAttachShader(m_program_id, vertex_shader);
            glAttachShader(m_program_id, fragment_shader);

            glLinkProgram(m_program_id);

            GLint is_linked = 0;
            glGetProgramiv(m_program_id, GL_LINK_STATUS, (int *)& is_linked);
            if (is_linked == GL_FALSE) {
                GLint log_length = 0;
                glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);

                GLchar *info_log = new GLchar[log_length];
                glGetShaderInfoLog(vertex_shader, log_length, &log_length, info_log);

                glDeleteProgram(m_program_id);
                glDeleteShader(vertex_shader);
                glDeleteShader(fragment_shader);

                HYP_CORE_ERROR("Shader linking error:\n{}", info_log);
                HYP_ASSERT_MESSAGE(false, "Shader linking failure!");

                delete info_log;

                return;
            }
        }

        glDetachShader(m_program_id, vertex_shader);
        glDetachShader(m_program_id, fragment_shader);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    COpenGLShader::~COpenGLShader() {
        glDeleteProgram(m_program_id);
    }

    void COpenGLShader::Bind() const {
        glUseProgram(m_program_id);
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

    s32 COpenGLShader::TryGetUniformLocation(const TString &name) {
        s32 location = glGetUniformLocation(m_program_id, name.c_str());
        if (location < 0) {
            HYP_CORE_ERROR("[OpenGL] - Failed to get location for uniform: {}", name);
        }
        return location;
    }

}