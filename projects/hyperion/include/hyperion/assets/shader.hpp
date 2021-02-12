#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/rendering/attributes/shader_attributes.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class IShaderRecompilationListener {
    public:
        virtual ~IShaderRecompilationListener() = default;
    public:
        virtual void OnRecompile() = 0;
    };

    class Shader : public Asset {
        HYP_REFLECT(Asset);
    public:
        inline AssetType GetAssetType() const override { return AssetType::Shader; }

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
        static Shader *Create();
    private:
        Vector<IShaderRecompilationListener *> m_recompilation_listeners;
    };

}