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

        {
            m_uniforms.clear();
            
            s32 uniform_count;
            glGetProgramiv(m_shader_program_id, GL_ACTIVE_UNIFORMS, &uniform_count);
            
            for (s32 i = 0; i < uniform_count; i++) {
                s32 uniform_name_length;
                char uniform_name[128];
                s32 uniform_size;
                u32 uniform_type;
                glGetActiveUniform(m_shader_program_id, i, 128, &uniform_name_length, &uniform_size, &uniform_type, uniform_name);

                s32 location = glGetUniformLocation(m_shader_program_id, uniform_name);
                if (location < 0) {
                    HYP_LOG_ERROR("OpenGL", "Failed to introspection location for uniform: '{}'!", uniform_name);
                }
                m_uniforms[uniform_name] = location;
            }
        }
    }

}