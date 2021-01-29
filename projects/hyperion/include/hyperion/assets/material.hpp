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

        Vec4 GetVec4(const String &name) const;
        void SetVec4(const String &name, Vec4 value);

        //Mat4 GetMat4(const String &name) const;
        void SetMat4(const String &name, const Mat4 &value);

        //Texture *GetTexture(const String &name) const;
        void SetTexture(const String &name, const Texture *value);

        static Material *Create(Shader *shader);
    protected:
        void OnDestroy() override;
    private:
        Material() = default;
        Material(Shader *shader);

        void SetProperty(const MaterialProperty &property);

        static Material *Create();
    private:
        Shader *m_shader;
        
        // RANT: The namespace 'Material' here is only necessary to shut up IntelliSense
        // that for some reason does not understand it properly otherwise.
        Vector<Material::MaterialProperty> m_properties;
    private:
        union MaterialPropertyStorage {
            float32 float32;
            int32 int32;

            Vec2 vec2;
            Vec3 vec3;
            Vec4 vec4;

            Mat3 mat3;
            Mat4 mat4;

            const Texture *texture;
        };

        struct MaterialProperty {
            String name;
            Rendering::MaterialPropertyType type;
            MaterialPropertyStorage storage;
        };
    };

}