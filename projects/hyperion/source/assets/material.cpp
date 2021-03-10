//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/material.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/render_engine.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Material::Material() {
        RegisterAsset();
    }

    //--------------------------------------------------------------
    Material::Material(Shader *shader) : Material() {
        m_shader = shader;
        
        Initialize();
    }

    //--------------------------------------------------------------
    float32 Material::GetFloat32(MaterialPropertyId id) const {
        auto it = m_properties.find(id);
        if (it != m_properties.end() && it->second.type == MaterialPropertyType::Float32) {
            return it->second.storage.float32;
        } else {
            return float32();
        }
    }

    //--------------------------------------------------------------
    void Material::SetFloat32(MaterialPropertyId id, float32 value) {
        auto it = m_properties.find(id);
        if (it != m_properties.end()) {
            HYP_ASSERT_MESSAGE(it->second.type == MaterialPropertyType::Float32, "The property can not change it's type when setting");
            it->second.storage.float32 = value;

            SendPropertyToGPU(id, it->second);
        } else {
            MaterialProperty &property = m_properties[id];
            property.type = MaterialPropertyType::Float32;
            property.storage.float32 = value;

            SendPropertyToGPU(id, property);
        }
    }

    //--------------------------------------------------------------
    int32 Material::GetInt32(MaterialPropertyId id) const {
        auto it = m_properties.find(id);
        if (it != m_properties.end() && it->second.type == MaterialPropertyType::Int32) {
            return it->second.storage.int32;
        } else {
            return int32();
        }
    }

    //--------------------------------------------------------------
    void Material::SetInt32(MaterialPropertyId id, int32 value) {
        auto it = m_properties.find(id);
        if (it != m_properties.end()) {
            HYP_ASSERT_MESSAGE(it->second.type == MaterialPropertyType::Int32, "The property can not change it's type when setting");
            it->second.storage.int32 = value;

            SendPropertyToGPU(id, it->second);
        } else {
            MaterialProperty &property = m_properties[id];
            property.type = MaterialPropertyType::Int32;
            property.storage.int32 = value;

            SendPropertyToGPU(id, property);
        }
    }

    //--------------------------------------------------------------
    uint32 Material::GetUInt32(Rendering::MaterialPropertyId id) const {
        auto it = m_properties.find(id);
        if (it != m_properties.end() && it->second.type == MaterialPropertyType::UInt32) {
            return it->second.storage.int32;
        } else {
            return uint32();
        }
    }

    //--------------------------------------------------------------
    void Material::SetUInt32(Rendering::MaterialPropertyId id, uint32 value) {
        auto it = m_properties.find(id);
        if (it != m_properties.end()) {
            HYP_ASSERT_MESSAGE(it->second.type == MaterialPropertyType::UInt32, "The property can not change it's type when setting");
            it->second.storage.unsigned_int32 = value;

            SendPropertyToGPU(id, it->second);
        } else {
            MaterialProperty &property = m_properties[id];
            property.type = MaterialPropertyType::UInt32;
            property.storage.unsigned_int32 = value;

            SendPropertyToGPU(id, property);
        }
    }

    //--------------------------------------------------------------
    Vec2 Material::GetVec2(MaterialPropertyId id) const {
        auto it = m_properties.find(id);
        if (it != m_properties.end() && it->second.type == MaterialPropertyType::Vec2) {
            return it->second.storage.vec2;
        } else {
            return Vec2();
        }
    }

    //--------------------------------------------------------------
    void Material::SetVec2(MaterialPropertyId id, Vec2 value) {
        auto it = m_properties.find(id);
        if (it != m_properties.end()) {
            HYP_ASSERT_MESSAGE(it->second.type == MaterialPropertyType::Vec2, "The property can not change it's type when setting");
            it->second.storage.vec2 = value;

            SendPropertyToGPU(id, it->second);
        } else {
            MaterialProperty &property = m_properties[id];
            property.type = MaterialPropertyType::Vec2;
            property.storage.vec2 = value;

            SendPropertyToGPU(id, property);
        }
    }

    //--------------------------------------------------------------
    Vec3 Material::GetVec3(MaterialPropertyId id) const {
        auto it = m_properties.find(id);
        if (it != m_properties.end() && it->second.type == MaterialPropertyType::Vec3) {
            return it->second.storage.vec3;
        } else {
            return Vec3();
        }
    }

    //--------------------------------------------------------------
    void Material::SetVec3(MaterialPropertyId id, Vec3 value) {
        auto it = m_properties.find(id);
        if (it != m_properties.end()) {
            HYP_ASSERT_MESSAGE(it->second.type == MaterialPropertyType::Vec3, "The property can not change it's type when setting");
            it->second.storage.vec3 = value;

            SendPropertyToGPU(id, it->second);
        } else {
            MaterialProperty &property = m_properties[id];
            property.type = MaterialPropertyType::Vec3;
            property.storage.vec3 = value;

            SendPropertyToGPU(id, property);
        }
    }

    //--------------------------------------------------------------
    Vec4 Material::GetVec4(MaterialPropertyId id) const {
        auto it = m_properties.find(id);
        if (it != m_properties.end() && it->second.type == MaterialPropertyType::Vec4) {
            return it->second.storage.vec4;
        } else {
            return Vec4();
        }
    }

    //--------------------------------------------------------------
    void Material::SetVec4(MaterialPropertyId id, Vec4 value) {
        auto it = m_properties.find(id);
        if (it != m_properties.end()) {
            HYP_ASSERT_MESSAGE(it->second.type == MaterialPropertyType::Vec4, "The property can not change it's type when setting");
            it->second.storage.vec4 = value;

            SendPropertyToGPU(id, it->second);
        } else {
            MaterialProperty &property = m_properties[id];
            property.type = MaterialPropertyType::Vec4;
            property.storage.vec4 = value;

            SendPropertyToGPU(id, property);
        }
    }

    //--------------------------------------------------------------
    Mat3 Material::GetMat3(MaterialPropertyId id) const {
        auto it = m_properties.find(id);
        if (it != m_properties.end() && it->second.type == MaterialPropertyType::Mat3) {
            return it->second.storage.mat3;
        } else {
            return Mat3();
        }
    }

    //--------------------------------------------------------------
    void Material::SetMat3(MaterialPropertyId id, const Mat3 &value) {
        auto it = m_properties.find(id);
        if (it != m_properties.end()) {
            HYP_ASSERT_MESSAGE(it->second.type == MaterialPropertyType::Mat3, "The property can not change it's type when setting");
            it->second.storage.mat3 = value;

            SendPropertyToGPU(id, it->second);
        } else {
            MaterialProperty &property = m_properties[id];
            property.type = MaterialPropertyType::Mat3;
            property.storage.mat3 = value;

            SendPropertyToGPU(id, property);
        }
    }

    //--------------------------------------------------------------
    Mat4 Material::GetMat4(MaterialPropertyId id) const {
        auto it = m_properties.find(id);
        if (it != m_properties.end() && it->second.type == MaterialPropertyType::Mat4) {
            return it->second.storage.mat4;
        } else {
            return Mat4();
        }
    }

    //--------------------------------------------------------------
    void Material::SetMat4(MaterialPropertyId id, const Mat4 &value) {
        auto it = m_properties.find(id);
        if (it != m_properties.end()) {
            HYP_ASSERT_MESSAGE(it->second.type == MaterialPropertyType::Mat4, "The property can not change it's type when setting");
            it->second.storage.mat4 = value;

            SendPropertyToGPU(id, it->second);
        } else {
            MaterialProperty &property = m_properties[id];
            property.type = MaterialPropertyType::Mat4;
            property.storage.mat4 = value;

            SendPropertyToGPU(id, property);
        }
    }

    //--------------------------------------------------------------
    Texture *Material::GetTexture(MaterialPropertyId id) const {
        // NOTE: We are also handling render textures.

        auto it = m_properties.find(id);
        if (it != m_properties.end()) {
            if (it->second.type == MaterialPropertyType::Texture) {
                return it->second.storage.texture;
            } else if (it->second.type == MaterialPropertyType::RenderTexture) {
                return it->second.storage.render_texture.render_texture;
            } else {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    void Material::SetTexture(MaterialPropertyId id, Texture *value) {
        HYP_ASSERT(value);

        // NOTE: We are also handling render textures as they are essentially textures as well.
        if (value->GetAssetType() == AssetType::RenderTexture) {
            SetRenderTexture(id, static_cast<RenderTexture *>(value), 0);
        } else {
            auto it = m_properties.find(id);
            if (it != m_properties.end()) {
                // Textures are a property which can change their type (either normal texture or render texture).
                HYP_ASSERT_MESSAGE(it->second.type == MaterialPropertyType::Texture || it->second.type == MaterialPropertyType::RenderTexture, "The property can not change it's type when setting");
                it->second.type = MaterialPropertyType::Texture;
                it->second.storage.texture = value;

                SendPropertyToGPU(id, it->second);
            } else {
                MaterialProperty &property = m_properties[id];
                property.type = MaterialPropertyType::Texture;
                property.storage.texture = value;

                SendPropertyToGPU(id, property);
            }
        }
    }

    //--------------------------------------------------------------
    RenderTexture *Material::GetRenderTexture(Rendering::MaterialPropertyId id) const {
        auto it = m_properties.find(id);
        if (it != m_properties.end() && it->second.type == MaterialPropertyType::RenderTexture) {
            return it->second.storage.render_texture.render_texture;
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    void Material::SetRenderTexture(Rendering::MaterialPropertyId id, RenderTexture *value, uint32 attachment_index) {
        HYP_ASSERT(value);

        auto it = m_properties.find(id);
        if (it != m_properties.end()) {
            // Textures are a property which can change their type (either normal texture or render texture).
            HYP_ASSERT_MESSAGE(it->second.type == MaterialPropertyType::RenderTexture || it->second.type == MaterialPropertyType::Texture, "The property can not change it's type when setting");
            it->second.type = MaterialPropertyType::RenderTexture;
            it->second.storage.render_texture.render_texture = value;
            it->second.storage.render_texture.attachment_index = attachment_index;

            SendPropertyToGPU(id, it->second);
        } else {
            MaterialProperty &property = m_properties[id];
            property.type = MaterialPropertyType::RenderTexture;
            property.storage.render_texture.render_texture = value;
            property.storage.render_texture.attachment_index = attachment_index;

            SendPropertyToGPU(id, property);
        }
    }

    //--------------------------------------------------------------
    void Material::Initialize() {
        m_shader->RegisterRecompilationListener(this);

        CreateOnGPU();
        SendPropertiesToGPU();
    }

    //--------------------------------------------------------------
    void Material::CreateOnGPU() {
        MaterialDescriptor descriptor = { };
        descriptor.shader_id = m_shader->GetResourceId();

        RenderEngine::GetRenderDriver()->CreateMaterial(m_resource_id, descriptor);
    }

    //--------------------------------------------------------------
    void Material::SendPropertyToGPU(MaterialPropertyId id, const MaterialProperty &property) {
        Rendering::MaterialProperty rendering_property { };
        rendering_property.id = id;
        rendering_property.type = property.type;
        
        switch (property.type) {
            case MaterialPropertyType::Float32: rendering_property.storage.float32 = property.storage.float32; break;
            case MaterialPropertyType::Int32: rendering_property.storage.int32 = property.storage.int32; break;
            case MaterialPropertyType::UInt32: rendering_property.storage.unsigned_int32 = property.storage.unsigned_int32; break;
            case MaterialPropertyType::Vec2: rendering_property.storage.vec2 = property.storage.vec2; break;
            case MaterialPropertyType::Vec3: rendering_property.storage.vec3 = property.storage.vec3; break;
            case MaterialPropertyType::Vec4: rendering_property.storage.vec4 = property.storage.vec4; break;
            case MaterialPropertyType::Mat3: rendering_property.storage.mat3 = property.storage.mat3; break;
            case MaterialPropertyType::Mat4: rendering_property.storage.mat4 = property.storage.mat4; break;
            case MaterialPropertyType::Texture: rendering_property.storage.texture = property.storage.texture->GetResourceId(); break;
            case MaterialPropertyType::RenderTexture: {
                rendering_property.storage.render_texture.render_texture = property.storage.render_texture.render_texture->GetResourceId();
                rendering_property.storage.render_texture.attachment_index = property.storage.render_texture.attachment_index;
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }

        RenderEngine::GetRenderDriver()->SetMaterialProperty(m_resource_id, rendering_property);
    }

    //--------------------------------------------------------------
    void Material::SendPropertiesToGPU() {
        for (auto &[material_property_id, material_property] : m_properties) {
            SendPropertyToGPU(material_property_id, material_property);
        }
    }

    //--------------------------------------------------------------
    Material *Material::Create() {
        return new Material();
    }

    //--------------------------------------------------------------
    void Material::OnRecompile() {
        SendPropertiesToGPU();
    }

    //--------------------------------------------------------------
    Material *Material::Create(Shader *shader) {
        return new Material(shader);
    }

    //--------------------------------------------------------------
    void Material::OnClone(Object *clone) {
        Asset::OnClone(clone);

        Material *material_clone = static_cast<Material *>(clone);
        material_clone->m_shader = m_shader;
        material_clone->m_properties = m_properties;

        material_clone->Initialize();
    }

    //--------------------------------------------------------------
    void Material::OnDestroy() {
        m_shader->UnregisterRecompilationListener(this);

        RenderEngine::GetRenderDriver()->DestroyMaterial(m_resource_id);

        Asset::OnDestroy();
    }

}
