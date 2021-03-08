#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/io/file_watcher.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Asset;
    class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class TexturePrimitive {
        Grid,
    };

    enum class MaterialPrimitive {
        Default,
        Ui,
        Font,
    };

    enum class MeshPrimitive {
        Quad,
        Plane,
        Cube,
        Sphere,
    };

    class AssetManager final {
    public:
        static Texture *GetTexturePrimitive(TexturePrimitive texture_primitive);
        static Material *GetMaterialPrimitive(MaterialPrimitive material_primitive);
        static Mesh *GetMeshPrimitive(MeshPrimitive mesh_primitive);
    private:
        ~AssetManager() = delete;
        AssetManager() = delete;
    private:
        static void Initialize(AssetSettings settings);
        static void Update();
        static void Shutdown();

        static void InitializeFileWatchers();
        static void InitializePrimitives();

        static void RegisterAsset(Asset *asset);
        static void UnregisterAsset(Asset *asset);
    private:
        inline static AssetSettings s_settings;

        inline static FileWatcher *s_file_watcher_shaders;

        inline static struct Primitives {
            Texture *texture_grid;

            Material *material_default;
            Material *material_ui;
            Material *material_font;

            Mesh *mesh_quad;
            Mesh *mesh_plane;
            Mesh *mesh_cube;
            Mesh *mesh_sphere;
        } s_primitives;

        inline static Map<ResourceId, Font *> s_fonts;
        inline static Map<ResourceId, Material *> s_materials;
        inline static Map<ResourceId, Mesh *> s_meshes;
        inline static Map<ResourceId, Shader *> s_shaders;
        inline static Map<ResourceId, Texture *> s_textures;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Asset;
    };

}