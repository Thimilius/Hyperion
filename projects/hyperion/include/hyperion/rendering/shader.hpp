#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/asset.hpp"
#include "hyperion/core/math/math.hpp"
#include "hyperion/core/io/file_watcher.hpp"

namespace Hyperion::Rendering {

    enum class ShaderType {
        None,
        Vertex,
        Fragment
    };

    class Shader : public Asset {
    public:
        Shader(const String &name) : Asset(name) { }
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual u32 GetID() const = 0;

        virtual void SetInt(const String &name, int value) = 0;

        virtual void SetFloat(const String &name, float value) = 0;
        virtual void SetFloat2(const String &name, const Vec2 &value) = 0;
        virtual void SetFloat3(const String &name, const Vec3 &value) = 0;
        virtual void SetFloat4(const String &name, const Vec4 &value) = 0;

        virtual void SetMat4(const String &name, const Mat4 &matrix) = 0;

        virtual void Recompile(const String &source) = 0;
        virtual void Recompile(const String &vertex_source, const String &fragment_source) = 0;

        static Ref<Shader> Create(const String &name, const String &source);
        static Ref<Shader> Create(const String &name, const String &vertex_source, const String &fragment_source);
    protected:
        ShaderType ShaderTypeFromString(const String &string);
    };

    class ShaderLibrary {
    private:
        struct ShaderEntry {
            Ref<Shader> shader;
            String filepath;
        };

        inline static Map<String, ShaderEntry> s_shaders;
        inline static Ref<FileWatcher> s_watcher;
    public:
        static void Init(const String &path);
        static Ref<Shader> Load(const String &name, const String &filepath);

        static void Add(const String &name, const String &filepath, const Ref<Shader> &shader);
        static Ref<Shader> Get(const String &name);

        static void Update();
    private:
        ShaderLibrary() = delete;
        ~ShaderLibrary() = delete;

        static void Reload(const String &name);
    };

}