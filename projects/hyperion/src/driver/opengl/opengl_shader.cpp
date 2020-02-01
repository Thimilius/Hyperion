#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_shader.hpp"

#include "hyperion/assets/asset_library.hpp"
#include "hyperion/core/io/file_utilities.hpp"

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
        Map<ShaderType, String> sources = PreProcess(source);

        // For now this is hardcoded to be always 2 elements big
        if (sources.size() != 2) {
            CompileFallbackShader();
        } else {
            Compile(sources);
        }
    }

    void OpenGLShader::Recompile(const String &vertex_source, const String &fragment_source) {
        Map<ShaderType, String> sources(2);
        sources[ShaderType::Vertex] = vertex_source;
        sources[ShaderType::Fragment] = fragment_source;

        Compile(sources);
    }

    Map<ShaderType, String> OpenGLShader::PreProcess(const String &source) {
        Map<ShaderType, String> sources;

        const char *type_token = "#type ";
        const char *import_token = "#import ";
        u64 type_token_length = std::strlen(type_token);
        u64 import_token_length = std::strlen(import_token);

        // TODO: Correctly handle all error cases

        // Find and split the source into corresponding shader type
        u64 position = source.find(type_token, 0);
        while (position != String::npos) {
            String shader_source;
            ShaderType shader_type;

            u64 end_of_line = source.find_first_of("\r\n", position);
            HYP_ASSERT_MESSAGE(end_of_line != String::npos, "Shader syntax error!");
            u64 begin = position + type_token_length;
            String type_string = source.substr(begin, end_of_line - begin);
            shader_type = ShaderTypeFromString(type_string);
            if (shader_type == ShaderType::Unknown) {
                HYP_LOG_ERROR("OpenGL", "Invalid shader type specifier: '{}'!", type_string);
                return sources;
            }

            u64 next_line_position = source.find_first_not_of("\r\n", end_of_line);
            position = source.find(type_token, next_line_position);
            u64 end = position - (next_line_position == String::npos ? source.size() - 1 : next_line_position);

            shader_source = source.substr(next_line_position, end);

            // Find and handle import statements in source
            {
                u64 position = shader_source.find(import_token, 0);
                while (position != String::npos) {
                    u64 end_of_line = shader_source.find_first_of("\r\n", position);
                    HYP_ASSERT_MESSAGE(end_of_line != String::npos, "Shader syntax error!");
                    u64 begin = position + import_token_length;
                    u64 end = end_of_line - begin;

                    String import_string = shader_source.substr(begin, end_of_line - begin);
                    OpenGLShaderModule shader_module = ShaderModuleFromString(import_string);
                    if (shader_module == OpenGLShaderModule::Unknown) {
                        HYP_LOG_ERROR("OpenGL", "Invalid shader import module: '{}'!", import_string);
                        return sources;
                    }

                    shader_source = shader_source.replace(position, (begin - position) + end, s_modules[shader_module]);

                    position = shader_source.find(import_token, end_of_line);
                }
            }

            sources[shader_type] = shader_source;
        }

        // Check that we found both a vertex and fragment source
        if (sources.find(ShaderType::Vertex) == sources.end()) {
            HYP_LOG_ERROR("OpenGL", "Failed to find a vertex shader type specifier!");
            return sources;
        } else if (sources.find(ShaderType::Fragment) == sources.end()) {
            HYP_LOG_ERROR("OpenGL", "Failed to find a fragment shader type specifier!");
            return sources;
        }

        return sources;
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

    void OpenGLShader::Init() {
        // FIXME: There should probably a better and more global way of getting configuration stuff
        const String &shader_path = AssetLibrary::GetSettings().shader_path;

        // Load hardcoded modules
        s_modules[OpenGLShaderModule::BasicVertex] = FileUtilities::ReadTextFile(shader_path + "/modules/basic_vertex.glsl");
        s_modules[OpenGLShaderModule::BasicFragment] = FileUtilities::ReadTextFile(shader_path + "/modules/basic_fragment.glsl");
    }

    ShaderType OpenGLShader::ShaderTypeFromString(const String &string) {
        if (string == "vertex") {
            return ShaderType::Vertex;
        } else if (string == "fragment") {
            return ShaderType::Fragment;
        } else {
            return ShaderType::Unknown;
        }
    }

    OpenGLShaderModule OpenGLShader::ShaderModuleFromString(const String &string) {
        if (string == "\"basic_vertex\"") {
            return OpenGLShaderModule::BasicVertex;
        } else if (string == "\"basic_fragment\"") {
            return OpenGLShaderModule::BasicFragment;
        } else {
            return OpenGLShaderModule::Unknown;
        }
    }

    u32 OpenGLShader::GetGLShaderType(ShaderType type) {
        switch (type) {
            case ShaderType::Vertex: return GL_VERTEX_SHADER;
            case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}