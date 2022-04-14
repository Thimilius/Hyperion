//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/opengl/opengl_shader_compiler.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  OpenGLShaderCompilationResult OpenGLShaderCompiler::Compile(const char *vertex_source, const char *fragment_source) {
    OpenGLShaderCompilationResult result = { };
    result.success = false;

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    {
      const GLchar *vertex = vertex_source;
      glShaderSource(vertex_shader, 1, &vertex, 0);
      glCompileShader(vertex_shader);

      GLint is_compiled = 0;
      glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
      if (is_compiled == GL_FALSE) {
        GLint log_length = 0;
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);

        String info_log(log_length, '\0');
        glGetShaderInfoLog(vertex_shader, log_length, &log_length, info_log.data());

        glDeleteShader(vertex_shader);

        HYP_LOG_ERROR("OpenGL", "Shader compilation error in vertex shader:\n{}", info_log.c_str());

        return result;
      }
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    {
      const GLchar *fragment = fragment_source;
      glShaderSource(fragment_shader, 1, &fragment, 0);
      glCompileShader(fragment_shader);

      GLint is_compiled = 0;
      glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
      if (is_compiled == GL_FALSE) {
        GLint log_length = 0;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);

        String info_log(log_length, '\0');
        glGetShaderInfoLog(fragment_shader, log_length, &log_length, info_log.data());

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        HYP_LOG_ERROR("OpenGL", "Shader compilation error in fragment shader:\n{}", info_log.c_str());

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

        String info_log(log_length, '\0');
        glGetProgramInfoLog(program, log_length, &log_length, info_log.data());

        HYP_LOG_ERROR("OpenGL", "Shader linking error:\n{}", info_log.c_str());

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(program);

        return result;
      }
    }

    {
      glDetachShader(program, vertex_shader);
      glDetachShader(program, fragment_shader);
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
    }

    result.success = true;
    result.program = program;
    return result;
  }

}
