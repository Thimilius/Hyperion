#pragma once

#include "hyperion/common.hpp"
#include "hyperion/math/math.hpp"

namespace Hyperion::Rendering {

    enum class EShaderType {
        None,
        Vertex,
        Fragment
    };

    class CShader {
    public:
        virtual ~CShader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(const TString &name, int value) = 0;

        virtual void SetFloat(const TString &name, float value) = 0;
        virtual void SetFloat2(const TString &name, const Math::SVec2 &value) = 0;
        virtual void SetFloat3(const TString &name, const Math::SVec3 &value) = 0;
        virtual void SetFloat4(const TString &name, const Math::SVec4 &value) = 0;

        virtual void SetMat4(const TString &name, const Math::SMat4 &matrix) = 0;

        static TRef<CShader> Create(const TString &source);
        static TRef<CShader> Create(const TString &vertex_source, const TString &fragment_source);
    protected:
        EShaderType ShaderTypeFromString(const TString &string);
    };

    class CShaderLibrary {
    private:
        inline static TMap<TString, TRef<CShader>> m_shaders;
    public:
        static TRef<CShader> Load(const TString &name, const TString &filepath);

        static void Add(const TString &name, const TRef<CShader> &shader);
        static TRef<CShader> Get(const TString &name);
    private:
        CShaderLibrary() = delete;
        ~CShaderLibrary() = delete;
    };

}