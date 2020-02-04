#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_shader.hpp"

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"

#include <glad/glad.h>

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
        glUniform1i(TryGetUniformLocation(name), value);
    }

    void OpenGLShader::SetFloat(const String &name, f32 value) {
        glUniform1f(TryGetUniformLocation(name), value);
    }

    void OpenGLShader::SetVec2(const String &name, const Vec2 &value) {
        glUniform2f(TryGetUniformLocation(name), value.x, value.y);
    }

    void OpenGLShader::SetVec3(const String &name, const Vec3 &value) {
        glUniform3f(TryGetUniformLocation(name), value.x, value.y, value.z);
    }

    void OpenGLShader::SetVec4(const String &name, const Vec4 &value) {
        glUniform4f(TryGetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetMat4(const String &name, const Mat4 &matrix) {
        glUniformMatrix4fv(TryGetUniformLocation(name), 1, GL_FALSE, matrix.elements);
    }

    void OpenGLShader::Recompile(const String &source) {
        m_uniforms.clear();
        m_shader_program_id = OpenGLShaderCompiler::Compile(source);
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

}