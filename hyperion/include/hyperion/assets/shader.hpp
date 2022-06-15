//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/render/types/render_types_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class Shader final : public Asset {
    HYP_REFLECT(Asset)
  public:
    inline AssetType GetAssetType() const override { return AssetType::Shader; }

    inline bool8 IsValid() const { return m_is_valid; }
    inline const Rendering::ShaderData &GetData() const { return m_data; }

    void Recompile(const String &source);
  public:
    inline static Rendering::ShaderPropertyId PropertyToId(const String &name) { return Rendering::ShaderProperty::PropertyToId(name); }
  private:
    Shader(AssetMetadata metadata) : Asset(std::move(metadata)) { }
    Shader(AssetMetadata metadata, const String &source);
  private:
    void PreProcess(const String &source);
  private:
    bool8 m_is_valid = false;
    Rendering::ShaderData m_data;
  private:
    friend class Hyperion::AssetManager;
  };

}
