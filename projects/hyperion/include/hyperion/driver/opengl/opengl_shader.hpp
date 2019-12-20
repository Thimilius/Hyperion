#pragma once

#include "hyperion/rendering/shader.hpp"

namespace Hyperion::Rendering {

    class COpenGLShader : public CShader {
    private:
        u32 m_shader_program_id;
        TMap<TString, u32> m_uniforms;
    public:
        COpenGLShader(const TString &name, const TString &source);
        COpenGLShader(const TString &name, const TString &vertex_source, const TString &fragment_source);
        ~COpenGLShader();

        void Bind() const override;
        void Unbind() const override;

        u32 GetID() const override { return m_shader_program_id; }

        void SetInt(const TString &name, int value) override;

        void SetFloat(const TString &name, float value) override;
        void SetFloat2(const TString &name, const Math::SVec2 &value) override;
        void SetFloat3(const TString &name, const Math::SVec3 &value) override;
        void SetFloat4(const TString &name, const Math::SVec4 &value) override;

        void SetMat4(const TString &name, const Math::SMat4 &matrix) override;

        void Recompile(const TString &source) override;
        void Recompile(const TString &vertex_source, const TString &fragment_source) override;
    private:
        TMap<EShaderType, TString> PreProcess(const TString &source);
        void Compile(TMap<EShaderType, TString> sources);
        void CompileFallbackShader();

        u32 GetGLShaderType(EShaderType type);
        s32 TryGetUniformLocation(const TString &name);
    };

}