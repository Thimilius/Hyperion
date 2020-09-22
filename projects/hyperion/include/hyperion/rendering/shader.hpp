#pragma once

#include "hyperion/common.hpp"
#include "hyperion/assets/asset.hpp"
#include "hyperion/core/math/math.hpp"
#include "hyperion/core/math/mat3.hpp"

namespace Hyperion::Rendering {

    enum class ShaderType {
        Unknown,

        Vertex,
        Fragment
    };

    enum class ShaderLightMode {
        Unknown,

        None,
        Forward,
    };

    struct ShaderAttributes {
        ShaderLightMode light_mode = ShaderLightMode::None;
    };

    enum class ShaderDataType {
        Unknown,

        Int,
        Float,

        Vec2,
        Vec3,
        Vec4,

        Mat3,
        Mat4,

        Texture2D,
        TextureCubemap,
    };

    struct ShaderUniformDeclaration {
        String name;
        ShaderDataType type;
    };

    class Shader : public Asset {
    public:
        virtual ~Shader() = default;

        AssetType GetAssetType() const override { return AssetType::Shader; }

        ShaderAttributes GetAttributes() const { return m_attributes; }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual u32 GetID() const = 0;

        virtual const Vector<ShaderUniformDeclaration> &GetUniformDeclarations() const = 0;

        virtual void SetInt(const String &name, u32 value) = 0;
        virtual void SetFloat(const String &name, f32 value) = 0;
        virtual void SetVec2(const String &name, const Vec2 &value) = 0;
        virtual void SetVec3(const String &name, const Vec3 &value) = 0;
        virtual void SetVec4(const String &name, const Vec4 &value) = 0;
        virtual void SetMat3(const String &name, const Mat3 &matrix) = 0;
        virtual void SetMat4(const String &name, const Mat4 &matrix) = 0;

        virtual void Recompile(const String &source) = 0;

        static Ref<Shader> Create(const String &name, const String &source);
    protected:
        Shader(const String &name) : Asset(name) { }
    protected:
        ShaderAttributes m_attributes;
    };

}