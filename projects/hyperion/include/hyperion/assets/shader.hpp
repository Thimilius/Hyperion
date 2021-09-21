#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/render/types/render_types_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Shader final : public Asset {
    public:
        inline AssetType GetAssetType() const override { return AssetType::Shader; }

        inline const Rendering::ShaderData &GetData() const { return m_data; }
    public:
        inline static Rendering::ShaderPropertyId PropertyToId(const String &name) { return Rendering::ShaderProperty::PropertyToId(name); }
    private:
        Shader(AssetInfo info) : Asset(info) { }
        Shader(AssetInfo info, const String &source);
    private:
        void PreProcess(const String &source);
    private:
        Rendering::ShaderData m_data;
    private:
        friend class Hyperion::AssetManager;
    };

}