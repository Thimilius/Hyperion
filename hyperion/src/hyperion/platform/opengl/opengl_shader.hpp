#pragma once

#include "hyperion/rendering/shader.hpp"

namespace Hyperion::Rendering {

    class COpenGLShader : public CShader {
    private:
        u32 m_program_id;
    public:
        COpenGLShader(const CString &vertex_source, const CString &fragment_source);
        ~COpenGLShader();

        void Bind() const override;
        void Unbind() const override;

        void SetInt(const CString &name, int value) override;

        void SetFloat(const CString &name, float value) override;
        void SetFloat2(const CString &name, const Math::SVec2 &value) override;
        void SetFloat3(const CString &name, const Math::SVec3 &value) override;
        void SetFloat4(const CString &name, const Math::SVec4 &value) override;

        void SetMat4(const CString &name, const Math::SMat4 &matrix) override;
    private:
        s32 TryGetUniformLocation(const CString &name);
    };

}