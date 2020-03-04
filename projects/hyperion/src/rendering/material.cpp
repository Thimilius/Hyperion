#include "hyppch.hpp"

#include "hyperion/rendering/material.hpp"

namespace Hyperion::Rendering {

    Ref<Material> Material::Copy() const {
        Ref<Material> copy = Create(m_shader);
        copy->m_properties = m_properties;
        return copy;
    }

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

    void Material::SetMat3(const String &name, const Mat3 &value) {
        u32 property_index = FindOrAddProperty(name, MaterialPropertyType::Mat3);
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
        u32 property_index = FindOrAddProperty(name, MaterialPropertyType::Texture2D);
        m_properties[property_index].storage = texture;
    }

    void Material::Bind() {
        m_shader->Bind();

        u32 texture_index = 0;
        for (MaterialProperty &property : m_properties) {
            String &name = property.name;
            MaterialPropertyStorage &storage = property.storage;
            switch (property.type) {
                case MaterialPropertyType::Int: m_shader->SetInt(name, std::get<u32>(storage)); break;
                case MaterialPropertyType::Float: m_shader->SetFloat(name, std::get<f32>(storage)); break;
                case MaterialPropertyType::Vec2: m_shader->SetVec2(name, std::get<Vec2>(storage)); break;
                case MaterialPropertyType::Vec3: m_shader->SetVec3(name, std::get<Vec3>(storage)); break;
                case MaterialPropertyType::Vec4: m_shader->SetVec4(name, std::get<Vec4>(storage)); break;
                case MaterialPropertyType::Mat3: m_shader->SetMat3(name, std::get<Mat3>(storage)); break;
                case MaterialPropertyType::Mat4: m_shader->SetMat4(name, std::get<Mat4>(storage)); break;
                case MaterialPropertyType::Color: m_shader->SetVec4(name, std::get<Color>(storage)); break;

                case MaterialPropertyType::Texture2D: {
                    m_shader->SetInt(name, texture_index);
                    std::get<Ref<Texture2D>>(storage)->Bind(texture_index);
                    texture_index++;
                    break;
                }

                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }
        }
    }

    void Material::Unbind() {
        m_shader->Unbind();
    }

    Ref<Material> Material::Create(const Ref<Shader> &shader) {
        return Ref<Material>(new Material(shader));
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