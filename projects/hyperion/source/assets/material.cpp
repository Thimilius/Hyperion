#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/material.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

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
            } else {
                HYP_LOG_WARN("Material", "Trying to set color material property that is not a color.");
            }
        } else {
            HYP_LOG_WARN("Material", "Trying to set nonexistent material property.");
        }

        SetDirty();
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