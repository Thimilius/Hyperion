#pragma once

#include "hyperion/core/io/file_watcher.hpp"
#include "hyperion/core/image.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/texture_2d.hpp"
#include "hyperion/rendering/texture_cubemap.hpp"
#include "hyperion/rendering/mesh.hpp"

namespace Hyperion {

    enum class MeshPrimitive {
        Sphere,
        Cube,
        Plane
    };

    class AssetLibrary {
    private:
        template<typename T>
        struct AssetEntry {
            Ref<T> asset;
            String filepath;
        };

        inline static AssetSettings s_settings;
        
        inline static Map<String, AssetEntry<Rendering::Shader>> s_shaders;
        inline static Ref<FileWatcher> s_shader_watcher;

        inline static Map<String, AssetEntry<Rendering::Texture2D>> s_textures;
        inline static Ref<FileWatcher> s_texture_watcher;

        inline static Map<String, AssetEntry<Rendering::TextureCubemap>> s_texture_cubemaps;
        
        inline static Ref<Rendering::Mesh> s_mesh_primitive_sphere;
        inline static Ref<Rendering::Mesh> s_mesh_primitive_cube;
        inline static Ref<Rendering::Mesh> s_mesh_primitive_plane;
    public:
        inline static const AssetSettings &GetSettings() { return s_settings; }

        static Ref<Rendering::Shader> LoadShader(const String &name, const String &filepath);
        static void AddShader(const String &name, const String &filepath, const Ref<Rendering::Shader> &shader);
        static Ref<Rendering::Shader> GetShader(const String &name);

        static Ref<Rendering::Texture2D> LoadTexture2D(const String &name, const String &filepath);
        static Ref<Rendering::Texture2D> LoadTexture2D(const String &name, const String &filepath, Rendering::TextureParameters parameters);
        static void AddTexture2D(const String &name, const String &filepath, const Ref<Rendering::Texture2D> &texture);
        static Ref<Rendering::Texture2D> GetTexture2D(const String &name);

        static Ref<Rendering::TextureCubemap> LoadTextureCubemap(const String &name, const String &directory, const String &extension);
        static void AddTextureCubemap(const String &name, const Ref<Rendering::TextureCubemap> &texture_cubemap);
        static Ref<Rendering::TextureCubemap> GetTextureCubemap(const String &name);

        static Ref<Rendering::Mesh> GetMeshPrimitive(MeshPrimitive mesh_primitive);
    private:
        AssetLibrary() = delete;
        ~AssetLibrary() = delete;

        static void Init(const AssetSettings &settings);
        static void Update();
        static void Shutdown();

        static void InitShaders(const String &shader_path);
        static void ReloadShader(const String &name);

        static void InitTextures2D(const String &texture_path);
        static void ReloadTexture2D(const String &name);

        static void InitMeshPrimitives();

        static Rendering::TextureFormat GetTextureFormatFromImage(const Ref<Image> &image);

        friend class Engine;
    };

}