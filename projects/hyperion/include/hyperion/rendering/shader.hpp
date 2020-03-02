#pragma once

#include "hyperion/common.hpp"
#include "hyperion/assets/asset.hpp"
#include "hyperion/core/math/math.hpp"

namespace Hyperion::Rendering {

    enum class ShaderType {
        Unknown,
        Vertex,
        Fragment
    };

    enum class ShaderLightMode {
        Unknown,
        None,
        Forward, // TODO: We may want to differentiate between forward base and additional lights 
    };

    struct ShaderProperties {
        ShaderLightMode light_mode = ShaderLightMode::None;
    };

    class Shader : public Asset {
    protected:
        ShaderProperties m_properties;
    public:
        virtual ~Shader() = default;

        AssetType GetType() const override { return AssetType::Shader; }

        ShaderProperties GetProperties() const { return m_properties; }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual u32 GetID() const = 0;

        virtual void SetInt(const char *name, u32 value) = 0;
        virtual void SetFloat(const char *name, f32 value) = 0;
        virtual void SetVec2(const char *name, const Vec2 &value) = 0;
        virtual void SetVec3(const char *name, const Vec3 &value) = 0;
        virtual void SetVec4(const char *name, const Vec4 &value) = 0;
        virtual void SetMat3(const char *name, const Mat3 &matrix) = 0;
        virtual void SetMat4(const char *name, const Mat4 &matrix) = 0;

        virtual void Recompile(const String &source) = 0;

        static Ref<Shader> Create(const String &name, const String &source);
    protected:
        Shader(const String &name) : Asset(name) { }
    };

}