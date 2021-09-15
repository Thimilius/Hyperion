#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/material.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Color Material::GetColor(MaterialPropertyId id) const {
        auto it = std::find_if(m_properties.begin(), m_properties.end(), [id](const MaterialProperty &property) {
            return property.id == id;
        });

        if (it != m_properties.end()) {
            const MaterialProperty &property = *it;
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
        auto it = std::find_if(m_properties.begin(), m_properties.end(), [id](const MaterialProperty &property) {
            return property.id == id;
        });

        if (it != m_properties.end()) {
            MaterialProperty &property = *it;
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

        Threading::ScopeLock lock(GetLocker());
        m_properties = shader->GetDefaultProperties();

        SetDirty();
    }

}