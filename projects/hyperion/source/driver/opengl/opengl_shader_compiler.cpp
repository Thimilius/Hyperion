#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"

#include "hyperion/core/io/file_system.hpp"
#include "hyperion/driver/opengl/opengl_shader_pre_processor.hpp"
#include "hyperion/driver/opengl/opengl_utilities.hpp"

namespace Hyperion::Rendering {

    void OpenGLShaderCompiler::Init() {
        for (auto &entry : std::filesystem::directory_iterator("data/shaders/modules")) {
            auto &path = entry.path();
            if (path.extension() == ".shader_module") {
                String filename = path.filename().u8string();
                String module_name = filename.substr(0, filename.find_last_of('.'));
                s_modules[module_name] = FileSystem::ReadAllText(path.u8string());
            }
        }
    }

    OpenGLShaderCompilationResult OpenGLShaderCompiler::Compile(const String &source) {
        OpenGLShaderCompilationResult result;

        OpenGLShaderPreProcessor pre_processor(source);
        OpenGLShaderPreProcessResult pre_process_result = pre_processor.PreProcess();
        if (!pre_process_result.success) {
            return result;
        }

        // First we need to compile all shader stages separately.
        Vector<GLuint> shaders;
        for (auto [shader_type, shader_source] : pre_process_result.sources) {
            GLuint shader = glCreateShader(OpenGLUtilities::GetGLShaderType(shader_type));

            const GLchar *source = shader_source.c_str();
            glShaderSource(shader, 1, &source, 0);

            glCompileShader(shader);

            GLint is_compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
            if (is_compiled == GL_FALSE) {
                GLint log_length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

                String info_log(log_length, '\0');
                glGetShaderInfoLog(shader, log_length, &log_length, info_log.data());

                for (GLuint id : shaders) {
                    glDeleteShader(id);
                }
                glDeleteShader(shader);

                HYP_LOG_ERROR("OpenGL", "Shader compilation error:\n{}", info_log.data());

                return result;
            }

            shaders.push_back(shader);
        }

        // Now we can link all stages together.
        GLuint program = glCreateProgram();
        {
            for (GLuint id : shaders) {
                glAttachShader(program, id);
            }
            glLinkProgram(program);

            GLint is_linked = 0;
            glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
            if (is_linked == GL_FALSE) {
                GLint log_length = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

                String info_log(log_length, '\0');
                glGetProgramInfoLog(program, log_length, &log_length, info_log.data());

                for (GLuint id : shaders) {
                    glDeleteShader(id);
                }
                glDeleteProgram(program);

                HYP_LOG_ERROR("OpenGL", "Shader linking error:\n{}", info_log.data());
            }
        }

        for (GLuint shader : shaders) {
            glDetachShader(program, shader);
            glDeleteShader(shader);
        }

        result.success = true;
        result.program = program;
        return result;
    }

    String OpenGLShaderCompiler::GetShaderModuleFromName(const String &name) {
        if (s_modules.find(name) == s_modules.end()) {
            return String();
        }

        return s_modules[name];
    }

}