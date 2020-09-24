#pragma once

#include "hyperion/core/image.hpp"
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/io/file_watcher.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/texture_2d.hpp"
#include "hyperion/rendering/texture_cubemap.hpp"
#include "hyperion/rendering/mesh.hpp"
#include "hyperion/rendering/material.hpp"

namespace Hyperion {
    class Engine;
}

namespace Hyperion {

    enum class MeshPrimitive {
        Quad,
        Plane,
        Cube,
        Sphere
    };

    class AssetManager {
    public:
        inline static const AssetSettings &GetSettings() { return s_settings; }

        static Rendering::Shader *LoadShader(const String &name, const String &filepath);
        static void AddShader(const String &name, const String &filepath, Rendering::Shader *shader);
        static Rendering::Shader *GetShader(const String &name);

        static Rendering::Texture2D *LoadTexture2D(const String &name, const String &filepath, Rendering::TextureParameters parameters = Rendering::TextureParameters());
        static void AddTexture2D(const String &name, const String &filepath, Rendering::Texture2D *texture);
        static Rendering::Texture2D *GetTexture2D(const String &name);

        static Rendering::TextureCubemap *LoadTextureCubemap(const String &name, const String &directory, const String &extension, Rendering::TextureParameters parameters = Rendering::TextureParameters());
        static void AddTextureCubemap(const String &name, Rendering::TextureCubemap *texture_cubemap);
        static Rendering::TextureCubemap *GetTextureCubemap(const String &name);

        static Rendering::Mesh *GetMeshPrimitive(MeshPrimitive mesh_primitive);
        inline static Rendering::Texture2D *GetTexturePrimitive() { return s_texture_primitive; }

        inline static Rendering::Material *GetDefaultMaterial() { return s_default_material; }
    private:
        AssetManager() = delete;
        ~AssetManager() = delete;

        static void Init(const AssetSettings &settings);
        static void Update();
        static void Shutdown();

        static void InitShaders(const String &shader_path);
        static void ReloadShader(const String &name);

        static void InitTextures2D(const String &texture_path);
        static void ReloadTexture2D(const String &name);

        static void InitMeshPrimitives();

        static Rendering::TextureFormat GetTextureFormatFromImage(Image *image);
    private:
        template<typename T>
        struct AssetEntry {
            T *asset;
            String filepath;
        };

        struct MeshPrimitives {
            Rendering::Mesh *quad;
            Rendering::Mesh *plane;
            Rendering::Mesh *cube;
            Rendering::Mesh *sphere;
        };

        inline static AssetSettings s_settings;

        inline static Map<String, AssetEntry<Rendering::Shader>> s_shaders;
        inline static FileWatcher *s_shader_watcher;
        inline static Map<String, AssetEntry<Rendering::Texture2D>> s_textures;
        inline static FileWatcher *s_texture_watcher;
        inline static Map<String, AssetEntry<Rendering::TextureCubemap>> s_texture_cubemaps;

        inline static MeshPrimitives s_mesh_primitives;
        inline static Rendering::Texture2D *s_texture_primitive;
        inline static Rendering::Material *s_default_material;
    private:
        friend class Hyperion::Engine;
    };

}