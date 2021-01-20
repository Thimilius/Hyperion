#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"

namespace Hyperion::Rendering {

    OpenGLShaderCompilationResult OpenGLShaderCompiler::Compile(const char *vertex, const char *fragment) {
        OpenGLShaderCompilationResult result;

        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        {
            const GLchar *vertex_source = vertex;
            glShaderSource(vertex_shader, 1, &vertex_source, 0);
            glCompileShader(vertex_shader);

            GLint is_compiled = 0;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
            if (is_compiled == GL_FALSE) {
                GLint log_length = 0;
                glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);

                GLchar *info_log = new GLchar[log_length];
                glGetShaderInfoLog(vertex_shader, log_length, &log_length, info_log);

                HYP_LOG_ERROR("OpenGL", "Shader compilation error in vertex shader:\n{}", info_log);

                glDeleteShader(vertex_shader);
                delete[] info_log;

                return result;
            }
        }

        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        {
            const GLchar *fragment_source = fragment;
            glShaderSource(fragment_shader, 1, &fragment_source, 0);
            glCompileShader(fragment_shader);

            GLint is_compiled = 0;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
            if (is_compiled == GL_FALSE) {
                GLint log_length = 0;
                glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);

                GLchar *info_log = new GLchar[log_length];
                glGetShaderInfoLog(fragment_shader, log_length, &log_length, info_log);

                HYP_LOG_ERROR("OpenGL", "Shader compilation error in fragment shader:\n{}", info_log);

                glDeleteShader(vertex_shader);
                glDeleteShader(fragment_shader);
                delete[] info_log;

                return result;
            }
        }
        
        GLuint program = glCreateProgram();
        {
            glAttachShader(program, vertex_shader);
            glAttachShader(program, fragment_shader);
            glLinkProgram(program);

            GLint is_linked = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
            if (is_linked == GL_FALSE) {
                GLint log_length = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

                GLchar *info_log = new GLchar[log_length];
                glGetProgramInfoLog(program, log_length, &log_length, info_log);

                HYP_LOG_ERROR("OpenGL", "Shader linking error:\n{}", info_log);

                glDeleteShader(vertex_shader);
                glDeleteShader(fragment_shader);
                glDeleteProgram(program);
                delete[] info_log;

                return result;
            }
        }

        {
            glDetachShader(program, vertex_shader);
            glDetachShader(program, fragment_shader);
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);
        }

        result.succes = true;
        result.program = program;
        return result;
    }

}