#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/shader.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/rendering/attributes/material_attributes.hpp"

namespace Hyperion {

    class Material : public Asset {
        HYP_REFLECT(Asset);
    private:
        union MaterialPropertyStorage;
        struct MaterialProperty;
    public:
        inline AssetType GetAssetType() const override { return AssetType::Material; }

        float32 GetFloat32(Rendering::MaterialPropertyId id) const;
        inline float32 GetFloat32(const String &name) const {
            return GetFloat32(Rendering::MaterialProperty::NameToId(name));
        }
        void SetFloat32(Rendering::MaterialPropertyId id, float32 value);
        inline void SetFloat32(const String &name, float32 value) {
            SetFloat32(Rendering::MaterialProperty::NameToId(name), value);
        }

        int32 GetInt32(Rendering::MaterialPropertyId id) const;
        inline int32 GetInt32(const String &name) const {
            return GetInt32(Rendering::MaterialProperty::NameToId(name));
        }
        void SetInt32(Rendering::MaterialPropertyId id, int32 value);
        inline void SetInt32(const String &name, int32 value) {
            SetInt32(Rendering::MaterialProperty::NameToId(name), value);
        }

        Vec2 GetVec2(Rendering::MaterialPropertyId id) const;
        inline Vec2 GetVec2(const String &name) const {
            return GetVec2(Rendering::MaterialProperty::NameToId(name));
        }
        void SetVec2(Rendering::MaterialPropertyId id, Vec2 value);
        inline void SetVec2(const String &name, Vec2 value) {
            SetVec2(Rendering::MaterialProperty::NameToId(name), value);
        }

        Vec3 GetVec3(Rendering::MaterialPropertyId id) const;
        inline Vec3 GetVec3(const String &name) const {
            return GetVec3(Rendering::MaterialProperty::NameToId(name));
        }
        void SetVec3(Rendering::MaterialPropertyId id, Vec3 value);
        inline void SetVec3(const String &name, Vec3 value) {
            SetVec3(Rendering::MaterialProperty::NameToId(name), value);
        }

        Vec4 GetVec4(Rendering::MaterialPropertyId id) const;
        inline Vec4 GetVec4(const String &name) const {
            return GetVec4(Rendering::MaterialProperty::NameToId(name));
        }
        void SetVec4(Rendering::MaterialPropertyId id, Vec4 value);
        inline void SetVec4(const String &name, Vec4 value) {
            SetVec4(Rendering::MaterialProperty::NameToId(name), value);
        }

        Mat3 GetMat3(Rendering::MaterialPropertyId id) const;
        inline Mat3 GetMat3(const String &name) const {
            return GetMat3(Rendering::MaterialProperty::NameToId(name));
        }
        void SetMat3(Rendering::MaterialPropertyId id, const Mat3 &value);
        inline void SetMat3(const String &name, const Mat3 &value) {
            SetMat3(Rendering::MaterialProperty::NameToId(name), value);
        }

        Mat4 GetMat4(Rendering::MaterialPropertyId id) const;
        inline Mat4 GetMat4(const String &name) const {
            return GetMat4(Rendering::MaterialProperty::NameToId(name));
        }
        void SetMat4(Rendering::MaterialPropertyId id, const Mat4 &value);
        inline void SetMat4(const String &name, const Mat4 &value) {
            SetMat4(Rendering::MaterialProperty::NameToId(name), value);
        }

        Texture *GetTexture(Rendering::MaterialPropertyId id) const;
        inline Texture *GetTexture(const String &name) const {
            return GetTexture(Rendering::MaterialProperty::NameToId(name));
        }
        void SetTexture(Rendering::MaterialPropertyId id, Texture *value);
        inline void SetTexture(const String &name, Texture *value) {
            SetTexture(Rendering::MaterialProperty::NameToId(name), value);
        }

        static Material *Create(Shader *shader);
    protected:
        void OnDestroy() override;
    private:
        Material() = default;
        Material(Shader *shader);

        void SetProperty(Rendering::MaterialPropertyId id, const MaterialProperty &property);

        static Material *Create();
    private:
        Shader *m_shader;
        
        // RANT: The namespace 'Material' here is only necessary to shut up IntelliSense
        // that for some reason does not understand it properly otherwise.
        Map<Rendering::MaterialPropertyId, MaterialProperty> m_properties;
    private:
        union MaterialPropertyStorage {
            float32 float32;
            int32 int32;

            Vec2 vec2;
            Vec3 vec3;
            Vec4 vec4;

            Mat3 mat3;
            Mat4 mat4;

            Texture *texture;
        };

        struct MaterialProperty {
            Rendering::MaterialPropertyType type;
            MaterialPropertyStorage storage = { };
        };
    };

}