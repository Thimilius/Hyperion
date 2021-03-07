#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/rendering/attributes/shader_attributes.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
    struct ShaderDescriptor;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class IShaderRecompilationListener {
    public:
        virtual ~IShaderRecompilationListener() = default;
    public:
        virtual void OnRecompile() = 0;
    };

    class Shader final : public Asset {
        HYP_REFLECT(Asset);
    private:
        using ShaderPreProcessCallback = std::function<void (const Rendering::ShaderDescriptor &)>;
    public:
        inline AssetType GetAssetType() const override { return AssetType::Shader; }

        inline Rendering::ShaderAttributes GetAttributes() const { return m_attributes; }

        void RegisterRecompilationListener(IShaderRecompilationListener *recompilation_listener);
        void UnregisterRecompilationListener(IShaderRecompilationListener *recompilation_listener);

        void Recompile(const String &source);
    public:
        static Shader *Create(const String &source);
    protected:
        void OnDestroy() override;
    private:
        Shader() = default;
        Shader(const String &source);
    private:
        void PreProcess(const String &source, ShaderPreProcessCallback callback);
    private:
        static Shader *Create();
    private:
        Rendering::ShaderAttributes m_attributes;

        Vector<IShaderRecompilationListener *> m_recompilation_listeners;
    };

}