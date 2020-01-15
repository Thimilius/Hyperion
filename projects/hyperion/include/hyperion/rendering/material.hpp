#pragma once

#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/texture_2d.hpp"
#include "hyperion/rendering/texture_cubemap.hpp"
#include "hyperion/core/math/mat4.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/color.hpp"

#include <variant>

namespace Hyperion::Rendering {

    class Material : public Asset {
        using MaterialPropertyStorage = std::variant<u32, f32, Vec2, Vec3, Vec4, Mat4, Color>;
    private:
        enum class MaterialPropertyType {
            Int,
            Float,

            Vec2,
            Vec3,
            Vec4,

            Mat4,

            Color
        };

        // TODO: Better include of textures in material properties
        struct MaterialProperty {
            MaterialPropertyType type;
            String name;
            MaterialPropertyStorage storage;
        };

        Ref<Shader> m_shader;
        Vector<MaterialProperty> m_properties;
        Map<String, Ref<Texture2D>> m_textures;
    public:
        AssetType GetType() const override { return AssetType::Material; }

        Ref<Shader> GetShader() const { return m_shader; }

        void SetInt(const String &name, u32 value);
        void SetFloat(const String &name, f32 value);
        void SetVec2(const String &name, const Vec2 &value);
        void SetVec3(const String &name, const Vec3 &value);
        void SetVec4(const String &name, const Vec4 &value);
        void SetMat4(const String &name, const Mat4 &value);
        void SetColor(const String &name, const Color &color);
        void SetTexture2D(const String &name, const Ref<Texture2D> &texture);

        void Bind();
        void Unbind();

        static Ref<Material> Create(const Ref<Shader> &shader);
    private:
        Material(const Ref<Shader> &shader);

        u32 FindOrAddProperty(const String &name, MaterialPropertyType type);
    };

}