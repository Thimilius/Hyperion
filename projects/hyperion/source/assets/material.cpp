//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/material.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    float32 Material::GetFloat(ShaderPropertyId id) const {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            const MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == ShaderPropertyType::Float) {
                return property.storage.float32;
            } else {
                HYP_LOG_WARN("Material", "Trying to get float material property that is not a float.");
                return float32();
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to get nonexistent material property.");
            return float32();
        }
    }

    //--------------------------------------------------------------
    void Material::SetFloat(ShaderPropertyId id, float32 value) {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == ShaderPropertyType::Float) {
                property.storage.float32 = value;
                SetDirty();
            } else {
                HYP_LOG_WARN("Material", "Trying to set float material property that is not a float.");
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to set nonexistent material property.");
        }
        
    }

    //--------------------------------------------------------------
    int32 Material::GetInt(Rendering::ShaderPropertyId id) const {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            const MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == ShaderPropertyType::Int) {
                return property.storage.int32;
            } else {
                HYP_LOG_WARN("Material", "Trying to get int material property that is not an int.");
                return int32();
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to get nonexistent material property.");
            return int32();
        }
    }

    //--------------------------------------------------------------
    void Material::SetInt(Rendering::ShaderPropertyId id, int32 value) {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == ShaderPropertyType::Int) {
                property.storage.int32 = value;
                SetDirty();
            } else {
                HYP_LOG_WARN("Material", "Trying to set int material property that is not an int.");
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to set nonexistent material property.");
        }
    }

    //--------------------------------------------------------------
    Vector4 Material::GetVector(Rendering::ShaderPropertyId id) const {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            const MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == ShaderPropertyType::Vector) {
                return property.storage.vector4;
            } else {
                HYP_LOG_WARN("Material", "Trying to get vector material property that is not a vector.");
                return Vector4();
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to get nonexistent material property.");
            return Vector4();
        }
    }

    //--------------------------------------------------------------
    void Material::SetVector(Rendering::ShaderPropertyId id, Vector4 value) {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == ShaderPropertyType::Vector) {
                property.storage.vector4 = value;
                SetDirty();
            } else {
                HYP_LOG_WARN("Material", "Trying to set vector material property that is not a vector.");
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to set nonexistent material property.");
        }
    }

    //--------------------------------------------------------------
    Color Material::GetColor(ShaderPropertyId id) const {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            const MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == ShaderPropertyType::Color) {
                return property.storage.color;
            } else {
                HYP_LOG_WARN("Material", "Trying to get color material property that is not a color.");
                return Color();
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to get nonexistent material property.");
            return Color();
        }
    }
    
    //--------------------------------------------------------------
    void Material::SetColor(ShaderPropertyId id, Color value) {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == ShaderPropertyType::Color) {
                property.storage.color = value;
                SetDirty();
            } else {
                HYP_LOG_WARN("Material", "Trying to set color material property that is not a color.");
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to set nonexistent material property.");
        }
    }

    //--------------------------------------------------------------
    Matrix4x4 Material::GetMatrix(Rendering::ShaderPropertyId id) const {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            const MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == ShaderPropertyType::Matrix) {
                return property.storage.matrix4x4;
            } else {
                HYP_LOG_WARN("Material", "Trying to get matrix material property that is not a matrix.");
                return Matrix4x4();
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to get nonexistent material property.");
            return Matrix4x4();
        }
    }

    //--------------------------------------------------------------
    void Material::SetMatrix(Rendering::ShaderPropertyId id, const Matrix4x4 &value) {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == ShaderPropertyType::Matrix) {
                property.storage.matrix4x4 = value;
                SetDirty();
            } else {
                HYP_LOG_WARN("Material", "Trying to set matrix material property that is not a matrix.");
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to set nonexistent material property.");
        }
    }

    //--------------------------------------------------------------
    Texture2D *Material::GetTexture(Rendering::ShaderPropertyId id) const {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            const MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == ShaderPropertyType::Texture) {
                return AssetManager::GetTexture2DById(property.storage.texture);
            } else {
                HYP_LOG_WARN("Material", "Trying to get texture material property that is not a texture.");
                return nullptr;
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to get nonexistent material property.");
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    void Material::SetTexture(Rendering::ShaderPropertyId id, const Texture2D *value) {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == ShaderPropertyType::Texture) {
                if (value == nullptr) {
                    HYP_LOG_WARN("Material", "Trying to set texture material property with a null texture.");
                } else {
                    property.storage.texture = value->GetAssetInfo().id;
                    SetDirty();
                }
            } else {
                HYP_LOG_WARN("Material", "Trying to set texture material property that is not a texture.");
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to set nonexistent material property.");
        }
    }

    //--------------------------------------------------------------
    void Material::ResetToDefaults() {
        const ShaderPropertyCollection &shader_properties = m_shader->GetData().properties;
        uint64 shader_property_count = shader_properties.GetLength();

        m_properties.Resize(shader_property_count);
        m_property_indices.Reserve(shader_property_count);

        for (MaterialPropertyIndex i = 0; i < shader_property_count; i++) {
            const ShaderProperty &shader_property = shader_properties[i];
            MaterialProperty &material_property = m_properties[i];

            material_property.id = shader_property.id;
            material_property.type = shader_property.type;
            material_property.storage = shader_property.storage;

            m_property_indices.Insert(shader_property.id, i);
        }

        SetDirty();
    }

    //--------------------------------------------------------------
    Material::Material(AssetInfo info, Shader *shader) : Asset(info) {
        m_shader = shader;
        ResetToDefaults();

        SetDirty();
    }

}