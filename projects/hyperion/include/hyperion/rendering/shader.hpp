#pragma once

#include "hyperion/common.hpp"
#include "hyperion/assets/asset.hpp"
#include "hyperion/core/math/math.hpp"

namespace Hyperion::Rendering {

    enum class ShaderType {
        None,
        Vertex,
        Fragment
    };

    class Shader : public Asset {
    public:
        virtual ~Shader() = default;

        AssetType GetType() const override { return AssetType::Shader; }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual u32 GetID() const = 0;

        virtual void SetInt(const String &name, u32 value) = 0;

        virtual void SetFloat(const String &name, f32 value) = 0;
        virtual void SetFloat2(const String &name, const Vec2 &value) = 0;
        virtual void SetFloat3(const String &name, const Vec3 &value) = 0;
        virtual void SetFloat4(const String &name, const Vec4 &value) = 0;

        virtual void SetMat4(const String &name, const Mat4 &matrix) = 0;

        virtual void Recompile(const String &source) = 0;
        virtual void Recompile(const String &vertex_source, const String &fragment_source) = 0;

        static Ref<Shader> Create(const String &name, const String &source);
        static Ref<Shader> Create(const String &name, const String &vertex_source, const String &fragment_source);
    protected:
        Shader(const String &name) : Asset(name) { }

        ShaderType ShaderTypeFromString(const String &string);
    };

}