#pragma once

#include "hyperion/core/io/file_watcher.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/texture.hpp"

namespace Hyperion {

    class AssetLibrary {
    private:
        template<typename T>
        struct AssetEntry {
            Ref<T> asset;
            String filepath;
        };

        inline static bool s_hot_loading;
        
        inline static Map<String, AssetEntry<Rendering::Shader>> s_shaders;
        inline static Ref<FileWatcher> s_shader_watcher;

        inline static Map<String, AssetEntry<Rendering::Texture2D>> s_textures;
        inline static Ref<FileWatcher> s_texture_watcher;
    public:
        static Ref<Rendering::Shader> LoadShader(const String &name, const String &filepath);
        static void AddShader(const String &name, const String &filepath, const Ref<Rendering::Shader> &shader);
        static Ref<Rendering::Shader> GetShader(const String &name);

        static Ref<Rendering::Texture2D> LoadTexture2D(const String &name, const String &filepath);
        static void AddTexture2D(const String &name, const String &filepath, const Ref<Rendering::Texture2D> &texture);
        static Ref<Rendering::Texture2D> GetTexture2D(const String &name);
    private:
        AssetLibrary() = delete;
        ~AssetLibrary() = delete;

        static void Init(bool hot_loading, const String &shader_path, const String &texture_path);
        static void Update();
        static void Shutdown();

        static void InitShaders(const String &shader_path);
        static void ReloadShader(const String &name);

        static void InitTextures2D(const String &texture_path);
        static void ReloadTexture2D(const String &name);

        friend class Engine;
    };

}