#include "hyppch.hpp"

#include "hyperion/rendering/material.hpp"

#include "hyperion/assets/asset_manager.hpp"

namespace Hyperion::Rendering {

    Ref<Material> Material::Copy() const {
        // TODO: We are doing an unnecessary default initilization of material properties when creating the copy.
        Ref<Material> copy = Create(m_shader);
        copy->m_properties = m_properties;
        return copy;
    }

    void Material::SetInt(const String &name, u32 value) {
        s32 property_index = FindProperty(name, ShaderDataType::Int);
        if (property_index >= 0) {
            m_properties[property_index].storage = value;
        }
    }

    void Material::SetFloat(const String &name, f32 value) {
        s32 property_index = FindProperty(name, ShaderDataType::Float);
        if (property_index >= 0) {
            m_properties[property_index].storage = value;
        }
    }

    void Material::SetVec2(const String &name, const Vec2 &value) {
        s32 property_index = FindProperty(name, ShaderDataType::Vec2);
        if (property_index >= 0) {
            m_properties[property_index].storage = value;
        }
    }

    void Material::SetVec3(const String &name, const Vec3 &value) {
        s32 property_index = FindProperty(name, ShaderDataType::Vec3);
        if (property_index >= 0) {
            m_properties[property_index].storage = value;
        }
    }

    void Material::SetVec4(const String &name, const Vec4 &value) {
        s32 property_index = FindProperty(name, ShaderDataType::Vec4);
        if (property_index >= 0) {
            m_properties[property_index].storage = value;
        }
    }

    void Material::SetMat3(const String &name, const Mat3 &value) {
        s32 property_index = FindProperty(name, ShaderDataType::Mat3);
        if (property_index >= 0) {
            m_properties[property_index].storage = value;
        }
    }

    void Material::SetMat4(const String &name, const Mat4 &value) {
        s32 property_index = FindProperty(name, ShaderDataType::Mat4);
        if (property_index >= 0) {
            m_properties[property_index].storage = value;
        }
    }

    void Material::SetColor(const String &name, const Color &color) {
        s32 property_index = FindProperty(name, ShaderDataType::Vec4);
        if (property_index >= 0) {
            m_properties[property_index].storage = Vec4(color.r, color.g, color.b, color.a);
        }
    }

    void Material::SetTexture2D(const String &name, const Ref<Texture2D> &texture) {
        s32 property_index = FindProperty(name, ShaderDataType::Texture2D);
        if (property_index >= 0) {
            m_properties[property_index].storage = texture;
        }
    }

    void Material::Bind() {
        m_shader->Bind();

        u32 texture_index = 0;
        for (MaterialProperty &property : m_properties) {
            String &name = property.name;
            MaterialPropertyStorage &storage = property.storage;
            switch (property.type) {
                case ShaderDataType::Int: m_shader->SetInt(name, std::get<u32>(storage)); break;
                case ShaderDataType::Float: m_shader->SetFloat(name, std::get<f32>(storage)); break;
                case ShaderDataType::Vec2: m_shader->SetVec2(name, std::get<Vec2>(storage)); break;
                case ShaderDataType::Vec3: m_shader->SetVec3(name, std::get<Vec3>(storage)); break;
                case ShaderDataType::Vec4: m_shader->SetVec4(name, std::get<Vec4>(storage)); break;
                case ShaderDataType::Mat3: m_shader->SetMat3(name, std::get<Mat3>(storage)); break;
                case ShaderDataType::Mat4: m_shader->SetMat4(name, std::get<Mat4>(storage)); break;

                case ShaderDataType::Texture2D: {
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

    Material::Material(const Ref<Shader> &shader) {
        m_shader = shader;

        const Vector<ShaderUniformDeclaration> &uniform_declarations = shader->GetUniformDeclarations();
        m_properties.reserve(uniform_declarations.size());

        for (ShaderUniformDeclaration uniform_declaration : uniform_declarations) {
            MaterialProperty property;

            property.name = uniform_declaration.name;
            property.type = uniform_declaration.type;
            switch (uniform_declaration.type) {
                case ShaderDataType::Int: property.storage = (u32)0; break;
                case ShaderDataType::Float: property.storage = 0.0f; break;
                case ShaderDataType::Vec2: property.storage = Vec2(); break;
                case ShaderDataType::Vec3: property.storage = Vec3(); break;
                case ShaderDataType::Vec4: property.storage = Vec4(); break;
                case ShaderDataType::Mat3: property.storage = Mat3(); break; // FIXME: This should propably be set to a mat3 identity matrix.
                case ShaderDataType::Mat4: property.storage = Mat4::Identity(); break;
                // TODO: The textures should be white as a default.
                case ShaderDataType::Texture2D: property.storage = AssetManager::GetTexture2D("checkerboard"); break;
                case ShaderDataType::TextureCubemap: property.storage = (Ref<TextureCubemap>)nullptr; break;
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
            }

            m_properties.push_back(property);
        }
    }

    s32 Material::FindProperty(const String &name, ShaderDataType type) {
        for (u32 i = 0; i < m_properties.size(); i++) {
            if (m_properties[i].name == name) {
                if (m_properties[i].type == type) {
                    return i;
                } else {
                    HYP_ASSERT_MESSAGE(false, "Found material property with correct name but the types do not match!");
                }
            }
        }

        return -1;
    }

}