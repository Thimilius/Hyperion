#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/object/object.hpp"
#include "hyperion/core/object/resource.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
    class Material;
    class Mesh;
    class Texture;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class AssetType {
        Font,
        Material,
        Mesh,
        Shader,
        Texture,
        RenderTexture,
        TextureAtlas,
    };

    class Asset : public Object {
        HYP_REFLECT(Object);
    public:
        virtual ~Asset() override = default;
    public:
        virtual AssetType GetAssetType() const = 0;

        inline ResourceId GetResourceId() const { return m_resource_id; }
    protected:
        Asset();
    protected:
        ResourceId m_resource_id;
    };

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
        static void Initialize();
        static void Update();
        static void Shutdown();
    private:
        inline static Texture *s_texture_primitive_grid;

        inline static Material *s_material_primitive_default;
        inline static Material *s_material_primitive_ui;
        inline static Material *s_material_primitive_font;

        inline static Mesh *s_mesh_primitive_quad;
        inline static Mesh *s_mesh_primitive_plane;
        inline static Mesh *s_mesh_primitive_cube;
        inline static Mesh *s_mesh_primitive_sphere;
    private:
        friend class Engine;
    };
    
}