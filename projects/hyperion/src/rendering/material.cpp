#include "hyppch.hpp"

#include "hyperion/rendering/material.hpp"

namespace Hyperion::Rendering {

    void Material::SetInt(const String &name, u32 value) {
        u32 property_index = FindOrAddProperty(name, MaterialPropertyType::Int);
        m_properties[property_index].storage = value;
    }

    void Material::SetFloat(const String &name, f32 value) {
        u32 property_index = FindOrAddProperty(name, MaterialPropertyType::Float);
        m_properties[property_index].storage = value;
    }

    void Material::SetVec2(const String &name, const Vec2 &value) {
        u32 property_index = FindOrAddProperty(name, MaterialPropertyType::Vec2);
        m_properties[property_index].storage = value;
    }

    void Material::SetVec3(const String &name, const Vec3 &value) {
        u32 property_index = FindOrAddProperty(name, MaterialPropertyType::Vec3);
        m_properties[property_index].storage = value;
    }

    void Material::SetVec4(const String &name, const Vec4 &value) {
        u32 property_index = FindOrAddProperty(name, MaterialPropertyType::Vec4);
        m_properties[property_index].storage = value;
    }

    void Material::SetMat4(const String &name, const Mat4 &value) {
        u32 property_index = FindOrAddProperty(name, MaterialPropertyType::Mat4);
        m_properties[property_index].storage = value;
    }

    void Material::SetColor(const String &name, const Color &color) {
        u32 property_index = FindOrAddProperty(name, MaterialPropertyType::Color);
        m_properties[property_index].storage = color;
    }

    void Material::SetTexture2D(const String &name, const Ref<Texture2D> &texture) {
        m_textures[name] = texture;
    }

    void Material::Bind() {
        m_shader->Bind();

        // TODO: This should be made more efficient
        for (MaterialProperty &property : m_properties) {
            String &name = property.name;
            MaterialPropertyStorage &storage = property.storage;
            switch (property.type) {
                case MaterialPropertyType::Int: m_shader->SetInt(name, std::get<u32>(storage)); break;
                case MaterialPropertyType::Float: m_shader->SetFloat(name, std::get<f32>(storage)); break;
                case MaterialPropertyType::Vec2: m_shader->SetFloat2(name, std::get<Vec2>(storage)); break;
                case MaterialPropertyType::Vec3: m_shader->SetFloat3(name, std::get<Vec4>(storage)); break;
                case MaterialPropertyType::Vec4: m_shader->SetFloat4(name, std::get<Vec4>(storage)); break;
                case MaterialPropertyType::Mat4: m_shader->SetMat4(name, std::get<Mat4>(storage)); break;
                case MaterialPropertyType::Color: m_shader->SetFloat4(name, std::get<Color>(storage)); break;
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }
        }

        // TODO: Try to correctly determine indices of textures
        // Set all textures
        u32 index = 0;
        for (auto &pair : m_textures) {
            String name = pair.first;
            Ref<Texture2D> &texture = pair.second;

            m_shader->SetInt(name, index);
            texture->Bind(index);
        }
    }

    void Material::Unbind() {
        m_shader->Unbind();
    }

    Ref<Material> Material::Create(const Ref<Shader> &shader) {
        return Ref<Material>();
    }

    u32 Material::FindOrAddProperty(const String &name, MaterialPropertyType type) {
        // Try to find the property first because it might exist already
        for (u32 i = 0; i < m_properties.size(); i++) {
            if (m_properties[i].name == name) {
                if (m_properties[i].type == type) {
                    return i;
                } else {
                    HYP_ASSERT_MESSAGE(false, "Found material property with correct name but the types do not match!");
                }
            }
        }

        // We could not find it, so add a new property
        MaterialProperty property;
        property.type = type;
        property.name = name;
        m_properties.push_back(property);

        return (u32)(m_properties.size() - 1);
    }

    Material::Material(const Ref<Shader> &shader) {
        m_shader = shader;
    }

}