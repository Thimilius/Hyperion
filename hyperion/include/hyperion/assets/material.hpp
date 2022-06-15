//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/shader.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/render/types/render_types_material.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class Material final : public Asset {
    HYP_REFLECT(Asset)
  public:
    inline AssetType GetAssetType() const override { return AssetType::Material; }

    inline Shader *GetShader() const { return m_shader; }
    inline const Rendering::MaterialPropertyCollection &GetProperties() const { return m_properties; }

    float32 GetFloat(Rendering::ShaderPropertyId id) const;
    inline float32 GetFloat(const String &name) const { return GetFloat(Shader::PropertyToId(name)); }
    void SetFloat(Rendering::ShaderPropertyId id, float32 value);
    inline void SetFloat(const String &name, float32 value) { SetFloat(Shader::PropertyToId(name), value); }

    int32 GetInt(Rendering::ShaderPropertyId id) const;
    inline int32 GetInt(const String &name) const { return GetInt(Shader::PropertyToId(name)); }
    void SetInt(Rendering::ShaderPropertyId id, int32 value);
    inline void SetInt(const String &name, int32 value) { SetInt(Shader::PropertyToId(name), value); }

    Vector4 GetVector(Rendering::ShaderPropertyId id) const;
    inline Vector4 GetVector(const String &name) const { return GetVector(Shader::PropertyToId(name)); }
    void SetVector(Rendering::ShaderPropertyId id, Vector4 value);
    inline void SetVector(const String &name, Vector4 value) { SetVector(Shader::PropertyToId(name), value); }

    Color GetColor(Rendering::ShaderPropertyId id) const;
    inline Color GetColor(const String &name) const { return GetColor(Shader::PropertyToId(name)); }
    void SetColor(Rendering::ShaderPropertyId id, Color value);
    inline void SetColor(const String &name, Color value) { SetColor(Shader::PropertyToId(name), value); }

    Matrix4x4 GetMatrix(Rendering::ShaderPropertyId id) const;
    inline Matrix4x4 GetMatrix(const String &name) const { return GetMatrix(Shader::PropertyToId(name)); }
    void SetMatrix(Rendering::ShaderPropertyId id, const Matrix4x4 &value);
    inline void SetMatrix(const String &name, const Matrix4x4 &value) { SetMatrix(Shader::PropertyToId(name), value); }

    Texture *GetTexture(Rendering::ShaderPropertyId id) const;
    inline Texture *GetTexture(const String &name) const { return GetTexture(Shader::PropertyToId(name)); }
    void SetTexture(Rendering::ShaderPropertyId id, const Texture *value);
    inline void SetTexture(const String &name, const Texture *value) { SetTexture(Shader::PropertyToId(name), value); }
    void SetTexture(Rendering::ShaderPropertyId id, const RenderTexture *value, uint32 attachment_index);

    inline void SetTexture(const String &name, const RenderTexture *value, uint32 attachment_index) {
      SetTexture(Shader::PropertyToId(name), value, attachment_index);
    }

    void ResetToDefaults();
  private:
    Material(AssetMetadata metadata) : Asset(std::move(metadata)) { }
    Material(AssetMetadata metadata, Shader *shader);
  private:
    Shader *m_shader = nullptr;
    Rendering::MaterialPropertyIndices m_property_indices;
    Rendering::MaterialPropertyCollection m_properties;
  private:
    friend class Hyperion::AssetManager;
  };

}
