#pragma once

#include "hyperion/core/io/file_watcher.hpp"
#include "hyperion/rendering/shader.hpp"

namespace Hyperion {

    class AssetLibrary {
    private:
        inline static bool s_hot_loading;
        
        struct ShaderEntry {
            Ref<Rendering::Shader> shader;
            String filepath;
        };
        inline static Map<String, ShaderEntry> s_shaders;
        inline static Ref<FileWatcher> s_shader_watcher;
    public:
        static Ref<Rendering::Shader> LoadShader(const String &name, const String &filepath);

        static void AddShader(const String &name, const String &filepath, const Ref<Rendering::Shader> &shader);
        static Ref<Rendering::Shader> GetShader(const String &name);
    private:
        AssetLibrary() = delete;
        ~AssetLibrary() = delete;

        static void Init(bool hot_loading, const String &shader_path);
        static void Update();
        static void Shutdown();

        static void InitShaders(const String &shader_path);
        static void ReloadShader(const String &name);

        friend class Engine;
    };

}