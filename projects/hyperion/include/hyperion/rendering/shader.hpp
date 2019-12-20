#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/asset.hpp"
#include "hyperion/math/math.hpp"
#include "hyperion/io/file_watcher.hpp"

namespace Hyperion::Rendering {

    enum class EShaderType {
        None,
        Vertex,
        Fragment
    };

    class CShader : public CAsset {
    public:
        CShader(const TString &name) : CAsset(name) { }
        virtual ~CShader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual u32 GetID() const = 0;

        virtual void SetInt(const TString &name, int value) = 0;

        virtual void SetFloat(const TString &name, float value) = 0;
        virtual void SetFloat2(const TString &name, const Math::SVec2 &value) = 0;
        virtual void SetFloat3(const TString &name, const Math::SVec3 &value) = 0;
        virtual void SetFloat4(const TString &name, const Math::SVec4 &value) = 0;

        virtual void SetMat4(const TString &name, const Math::SMat4 &matrix) = 0;

        virtual void Recompile(const TString &source) = 0;
        virtual void Recompile(const TString &vertex_source, const TString &fragment_source) = 0;

        static TRef<CShader> Create(const TString &name, const TString &source);
        static TRef<CShader> Create(const TString &name, const TString &vertex_source, const TString &fragment_source);
    protected:
        EShaderType ShaderTypeFromString(const TString &string);
    };

    class CShaderLibrary {
    private:
        struct SShaderEntry {
            TRef<CShader> shader;
            TString filepath;
        };

        inline static TMap<TString, SShaderEntry> s_shaders;
        inline static TRef<IO::CFileWatcher> s_watcher;
    public:
        static void Init(const TString &path);
        static TRef<CShader> Load(const TString &name, const TString &filepath);

        static void Add(const TString &name, const TString &filepath, const TRef<CShader> &shader);
        static TRef<CShader> Get(const TString &name);

        static void Update();
    private:
        CShaderLibrary() = delete;
        ~CShaderLibrary() = delete;

        static void Reload(const TString &name);
    };

}