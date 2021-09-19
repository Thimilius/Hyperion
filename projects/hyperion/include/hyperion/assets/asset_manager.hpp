#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/rendering/types/render_types_mesh.hpp"

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

    enum class ShaderPrimitive {
        Standard,
        Gizmo,
    };

    enum class MaterialPrimitive {
        Default
    };

    enum class MeshPrimitive {
        Quad,
        Plane,
        Cube,
        Sphere,
    };

    class AssetManager final {
    public:
        static Texture2D *CreateTexture2D();

        static Shader *GetShaderPrimitive(ShaderPrimitive shader_primitive);
        static Shader *GetShaderByGuid(AssetGuid guid);
        static Shader *CreateShader(const String &source);

        static Material *GetMaterialPrimitive(MaterialPrimitive material_primitive);
        static Material *GetMaterialByGuid(AssetGuid guid);
        static Material *CreateMaterial(Shader *shader);

        static Mesh *GetMeshPrimitive(MeshPrimitive mesh_primitive);
        static Mesh *GetMeshByGuid(AssetGuid guid);
        static Mesh *CreateMesh();
        static Mesh *CreateMesh(const Rendering::MeshData &data, const Rendering::SubMeshes &sub_meshes, AssetDataAccess data_access = AssetDataAccess::None);

        static void Unload(Asset *asset);
    private:
        AssetManager() = delete;
        ~AssetManager() = delete;
    private:
        static void Initialize();
        static void LateUpdate();
        static void Shutdown();

        static void InitializePrimitives();
        static void SetNewGuid(Asset *asset, const String &guid);

        static void AddDirtyAsset(Asset *asset);

        static AssetInfo GetNextAssetInfo(AssetDataAccess data_access);
    private:
        inline static Map<AssetGuid, Material *> s_materials;
        inline static Map<AssetGuid, Mesh *> s_meshes;
        inline static Map<AssetGuid, Shader *> s_shaders;
        inline static Map<AssetGuid, Texture *> s_textures;

        inline static struct Primitives {
            Shader *shader_standard;
            Shader *shader_gizmo;

            Material *material_default;

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