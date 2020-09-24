#pragma once

#include <variant>

#include "hyperion/assets/copyable.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/texture_2d.hpp"
#include "hyperion/rendering/texture_cubemap.hpp"
#include "hyperion/core/math/mat4.hpp"
#include "hyperion/core/math/mat3.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/color.hpp"

namespace Hyperion::Rendering {

    class Material : public Asset, public ICopyable<Material> {
        using MaterialPropertyStorage = std::variant<u32, f32, Vec2, Vec3, Vec4, Mat3, Mat4, Texture2D *, TextureCubemap *>;
    private:
        struct MaterialProperty {
            ShaderDataType type;
            String name;
            MaterialPropertyStorage storage;
        };
    public:
        AssetType GetAssetType() const override { return AssetType::Material; }

        Material *Copy() const override;

        Shader* GetShader() const { return m_shader; }

        void SetInt(const String &name, u32 value);
        void SetFloat(const String &name, f32 value);
        void SetVec2(const String &name, const Vec2 &value);
        void SetVec3(const String &name, const Vec3 &value);
        void SetVec4(const String &name, const Vec4 &value);
        void SetMat3(const String &name, const Mat3 &value);
        void SetMat4(const String &name, const Mat4 &value);
        void SetColor(const String &name, const Color &color);
        void SetTexture2D(const String &name, Texture2D *texture);

        void Bind();
        void Unbind();

        static Material *Create(Shader *shader);
    private:
        Material(Shader *shader);

        s32 FindProperty(const String &name, ShaderDataType type);
    private:
        Shader *m_shader;
        Vector<MaterialProperty> m_properties;
    };

}