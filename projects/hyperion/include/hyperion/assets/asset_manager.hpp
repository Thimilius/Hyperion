#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/render/types/render_types_mesh.hpp"
#include "hyperion/render/types/render_types_texture.hpp"

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

    enum class Texture2DPrimitive {
        White,
    };

    enum class ShaderPrimitive {
        Standard,
        Unlit,
        Gizmo,
    };

    enum class MaterialPrimitive {
        Default,
        Unlit
    };

    enum class MeshPrimitive {
        Quad,
        Plane,
        Cube,
        Sphere,
    };

    class AssetManager final {
    public:
        static Texture2D *GetTexture2DPrimitive(Texture2DPrimitive texture_2d_primitive);
        static Texture2D *GetTexture2DByGuid(AssetGuid guid);
        static Texture2D *GetTexture2DById(AssetId id);
        static Texture2D *CreateTexture2D(const Rendering::Texture2DParameters &parameters);
        static Texture2D *CreateTexture2D(const Rendering::Texture2DParameters &parameters, const Rendering::TexturePixelData &pixels, AssetDataAccess data_access = AssetDataAccess::None);

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
            Texture2D *texture_2d_white;

            Shader *shader_standard;
            Shader *shader_unlit;
            Shader *shader_gizmo;

            Material *material_default;
            Material *material_unlit;

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