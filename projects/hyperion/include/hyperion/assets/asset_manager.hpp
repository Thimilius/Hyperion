#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class AssetLoadSystem;
    class AssetUnloadSystem;
    class Engine;
    class Material;
    class Mesh;
    class Shader;
    class Texture;
    class Texture2D;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class MeshPrimitive {
        Quad,
        Plane,
        Cube,
        Sphere,
    };

    class AssetManager final {
    public:
        static Material *CreateMaterial(Shader *shader);

        static Mesh *GetMeshPrimitive(MeshPrimitive mesh_primitive);
        static Mesh *CreateMesh();

        static Texture2D *CreateTexture2D();

        static Shader *CreateShader(const String &source);

        static void Unload(Asset *asset);
    private:
        AssetManager() = delete;
        ~AssetManager() = delete;
    private:
        static void Initialize();
        static void LateUpdate();
        static void Shutdown();

        static void InitializePrimitives();

        static void AddDirtyAsset(Asset *asset);

        static AssetInfo GetNextAssetInfo();
    private:
        inline static Map<AssetId, Material *> s_materials;
        inline static Map<AssetId, Mesh *> s_meshes;
        inline static Map<AssetId, Shader *> s_shaders;
        inline static Map<AssetId, Texture *> s_textures;

        inline static struct Primitives {
            Mesh *mesh_quad;
            Mesh *mesh_plane;
            Mesh *mesh_cube;
            Mesh *mesh_sphere;
        } s_primitives;

        inline static Array<Asset *> s_assets_to_load;
        inline static Array<Asset *> s_assets_to_unload;

        inline static AssetId s_id_counter;
    private:
        friend class Hyperion::Asset;
        friend class Hyperion::AssetLoadSystem;
        friend class Hyperion::AssetUnloadSystem;
        friend class Hyperion::Engine;
    };

}