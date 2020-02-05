#pragma once

#include "hyperion/rendering/shader.hpp"

namespace Hyperion::Rendering {

    class OpenGLShader : public Shader {
    private:
        u32 m_shader_program_id;
        Map<String, s32> m_uniforms;
    public:
        OpenGLShader(const String &name, const String &source);
        ~OpenGLShader() override;

        void Bind() const override;
        void Unbind() const override;

        u32 GetID() const override { return m_shader_program_id; }

        void SetInt(const String &name, u32 value) override;
        void SetFloat(const String &name, f32 value) override;
        void SetVec2(const String &name, const Vec2 &value) override;
        void SetVec3(const String &name, const Vec3 &value) override;
        void SetVec4(const String &name, const Vec4 &value) override;
        void SetMat3(const String &name, const Mat3 &matrix) override;
        void SetMat4(const String &name, const Mat4 &matrix) override;

        void Recompile(const String &source) override;
    private:
        s32 TryGetUniformLocation(const String &name);
    };

}