#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/material.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    float32 Material::GetFloat(MaterialPropertyId id) const {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            const MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == MaterialPropertyType::Float) {
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
    void Material::SetFloat(MaterialPropertyId id, float32 value) {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == MaterialPropertyType::Float) {
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
    int32 Material::GetInt(Rendering::MaterialPropertyId id) const {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            const MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == MaterialPropertyType::Int) {
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
    void Material::SetInt(Rendering::MaterialPropertyId id, int32 value) {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == MaterialPropertyType::Int) {
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
    Vector4 Material::GetVector(Rendering::MaterialPropertyId id) const {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            const MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == MaterialPropertyType::Vector) {
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
    void Material::SetVector(Rendering::MaterialPropertyId id, Vector4 value) {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == MaterialPropertyType::Vector) {
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
    Color Material::GetColor(MaterialPropertyId id) const {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            const MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == MaterialPropertyType::Color) {
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
    void Material::SetColor(MaterialPropertyId id, Color value) {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == MaterialPropertyType::Color) {
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
    Matrix4x4 Material::GetMatrix(Rendering::MaterialPropertyId id) const {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            const MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == MaterialPropertyType::Matrix) {
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
    void Material::SetMatrix(Rendering::MaterialPropertyId id, const Matrix4x4 &value) {
        auto it = m_property_indices.Find(id);
        if (it != m_property_indices.end()) {
            MaterialProperty &property = m_properties.Get(it->second);
            if (property.type == MaterialPropertyType::Matrix) {
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
    Material::Material(AssetInfo info, Shader *shader) : Asset(info) {
        m_shader = shader;

        m_properties = shader->GetDefaultProperties();
        MaterialPropertyIndex index = 0;
        for (const MaterialProperty &property : m_properties) {
            m_property_indices.Insert(property.id, index++);
        }

        SetDirty();
    }

}