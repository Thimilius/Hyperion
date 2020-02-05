#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"

#include "hyperion/driver/opengl/opengl_shader_pre_processor.hpp"
#include "hyperion/assets/asset_library.hpp"
#include "hyperion/core/io/file_utilities.hpp"

#include <glad/glad.h>
#include <sstream>

namespace Hyperion::Rendering {

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