#include "hyppch.hpp"

#include "hyperion/assets/material.hpp"

#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/render_engine.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion {

    Material::Material(Shader *shader) {
        m_shader = shader;

        MaterialDescriptor descriptor;
        descriptor.shader = shader->GetResourceId();

        RenderEngine::GetRenderDriver()->CreateMaterial(m_resource_id, descriptor);
    }

    Vec4 Material::GetVec4(const String &name) const {
        auto it = std::find_if(m_properties.begin(), m_properties.end(), [&name](const MaterialProperty property) {
            return property.name == name;
        });
        if (it != m_properties.end() && it->type == MaterialPropertyType::Vec4) {
            return it->storage.vec4;
        } else {
            return Vec4();
        }
    }

    void Material::SetVec4(const String &name, Vec4 value) {
        auto it = std::find_if(m_properties.begin(), m_properties.end(), [&name](const MaterialProperty property) {
            return property.name == name;
        });

        if (it != m_properties.end()) {
            HYP_ASSERT_MESSAGE(it->type == MaterialPropertyType::Vec4, "The property can not change it's type when setting");
            it->storage.vec4 = value;

            SetProperty(*it);
        } else {
            MaterialProperty property { };
            property.name = name;
            property.type = MaterialPropertyType::Vec4;
            property.storage.vec4 = value;

            m_properties.push_back(property);

            SetProperty(property);
        }
    }

    void Material::SetProperty(const MaterialProperty &property) {
        Rendering::MaterialProperty rendering_property { };
        rendering_property.name = property.name;
        rendering_property.type = property.type;
        
        switch (property.type) {
            case MaterialPropertyType::Float32: rendering_property.storage.float32 = property.storage.float32; break;
            case MaterialPropertyType::Int32: rendering_property.storage.int32 = property.storage.int32; break;
            case MaterialPropertyType::Vec2: rendering_property.storage.vec2 = property.storage.vec2; break;
            case MaterialPropertyType::Vec3: rendering_property.storage.vec3 = property.storage.vec3; break;
            case MaterialPropertyType::Vec4: rendering_property.storage.vec4 = property.storage.vec4; break;
            case MaterialPropertyType::Mat3: rendering_property.storage.mat3 = property.storage.mat3; break;
            case MaterialPropertyType::Mat4: rendering_property.storage.mat4 = property.storage.mat4; break;
            case MaterialPropertyType::Texture: rendering_property.storage.texture = property.storage.texture->GetResourceId(); break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }

        RenderEngine::GetRenderDriver()->SetMaterialProperty(m_resource_id, rendering_property);
    }

    Material *Material::Create() {
        return new Material();
    }

    Material *Material::Create(Shader *shader) {
        return new Material(shader);
    }

    void Material::OnDestroy() {
        RenderEngine::GetRenderDriver()->DestroyMaterial(m_resource_id);
    }

}
