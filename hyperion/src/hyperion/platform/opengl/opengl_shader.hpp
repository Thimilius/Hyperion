#pragma once

#include "hyperion/rendering/shader.hpp"

namespace Hyperion::Rendering {

    class COpenGLShader : public CShader {
    private:
        u32 m_program_id;
    public:
        COpenGLShader(const TString &source);
        COpenGLShader(const TString &vertex_source, const TString &fragment_source);
        ~COpenGLShader();

        void Bind() const override;
        void Unbind() const override;

        void SetInt(const TString &name, int value) override;

        void SetFloat(const TString &name, float value) override;
        void SetFloat2(const TString &name, const Math::SVec2 &value) override;
        void SetFloat3(const TString &name, const Math::SVec3 &value) override;
        void SetFloat4(const TString &name, const Math::SVec4 &value) override;

        void SetMat4(const TString &name, const Math::SMat4 &matrix) override;
    private:
        TMap<EShaderType, TString> PreProcess(const TString &source);
        void Compile(TMap<EShaderType, TString> sources);

        u32 GetGLShaderType(EShaderType type);
        s32 TryGetUniformLocation(const TString &name);
    };

}