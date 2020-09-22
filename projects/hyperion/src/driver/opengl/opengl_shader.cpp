#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_shader.hpp"

#include <glad/glad.h>

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"

namespace Hyperion::Rendering {

    OpenGLShader::OpenGLShader(const String &name, const String &source) : Shader(name) {
        Recompile(source);
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
        glProgramUniform1i(m_shader_program_id, m_uniforms.at(name), value);
    }

    void OpenGLShader::SetFloat(const String &name, f32 value) {
        glProgramUniform1f(m_shader_program_id, m_uniforms.at(name), value);
    }

    void OpenGLShader::SetVec2(const String &name, const Vec2 &value) {
        glProgramUniform2f(m_shader_program_id, m_uniforms.at(name), value.x, value.y);
    }

    void OpenGLShader::SetVec3(const String &name, const Vec3 &value) {
        glProgramUniform3f(m_shader_program_id, m_uniforms.at(name), value.x, value.y, value.z);
    }

    void OpenGLShader::SetVec4(const String &name, const Vec4 &value) {
        glProgramUniform4f(m_shader_program_id, m_uniforms.at(name), value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetMat3(const String &name, const Mat3 &matrix) {
        glProgramUniformMatrix3fv(m_shader_program_id, m_uniforms.at(name), 1, GL_FALSE, matrix.elements);
    }

    void OpenGLShader::SetMat4(const String &name, const Mat4 &matrix) {
        glProgramUniformMatrix4fv(m_shader_program_id, m_uniforms.at(name), 1, GL_FALSE, matrix.elements);
    }

    void OpenGLShader::Recompile(const String &source) {
        OpenGLShaderCompilerResult result = OpenGLShaderCompiler::Compile(source);
        m_shader_program_id = result.id;
        m_attributes = result.properties;

        CollectUniformDeclarations();
    }

    void OpenGLShader::CollectUniformDeclarations() {
        m_uniforms.clear();
        m_uniform_declarations.clear();

        s32 uniform_count;
        glGetProgramiv(m_shader_program_id, GL_ACTIVE_UNIFORMS, &uniform_count);

        m_uniforms.reserve(uniform_count);
        m_uniform_declarations.reserve(uniform_count);

        const u32 uniform_name_buffer_length = 128;
        for (s32 i = 0; i < uniform_count; i++) {
            s32 uniform_name_length;
            char uniform_name[uniform_name_buffer_length];
            s32 uniform_size;
            u32 uniform_type;
            glGetActiveUniform(m_shader_program_id, i, uniform_name_buffer_length, &uniform_name_length, &uniform_size, &uniform_type, uniform_name);

            if (uniform_name_length > uniform_name_buffer_length) {
                HYP_LOG_WARN("OpenGL", "The name of a shader uniform is longer than the buffer for the name and is therefore cut off!");
            }

            ShaderUniformDeclaration uniform_declaration;
            uniform_declaration.name = uniform_name;
            uniform_declaration.type = GetShaderDataType(uniform_type);
            m_uniform_declarations.push_back(uniform_declaration);

            s32 location = glGetUniformLocation(m_shader_program_id, uniform_name);
            if (location < 0) {
                HYP_LOG_ERROR("OpenGL", "Failed to introspection location for uniform: '{}'!", uniform_name);
            }
            m_uniforms[uniform_name] = location;
        }
    }

    ShaderDataType OpenGLShader::GetShaderDataType(u32 gl_type) {
        switch (gl_type) {
            case GL_INT: return ShaderDataType::Int;
            case GL_FLOAT: return ShaderDataType::Float;
            case GL_FLOAT_VEC2: return ShaderDataType::Vec2;
            case GL_FLOAT_VEC3: return ShaderDataType::Vec3;
            case GL_FLOAT_VEC4: return ShaderDataType::Vec4;
            case GL_FLOAT_MAT3: return ShaderDataType::Mat3;
            case GL_FLOAT_MAT4: return ShaderDataType::Mat4;
            case GL_SAMPLER_2D: return ShaderDataType::Texture2D;
            case GL_SAMPLER_CUBE: return ShaderDataType::TextureCubemap;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return ShaderDataType::Unknown;
        }
    }

}