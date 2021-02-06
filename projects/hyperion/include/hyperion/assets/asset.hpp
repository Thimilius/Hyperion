#pragma once

#include "hyperion/core/object/object.hpp"
#include "hyperion/core/object/resource.hpp"

namespace Hyperion {
    class Engine;
    class Material;
    class Mesh;
    class Texture;
}

namespace Hyperion {

    enum class AssetType {
        Font,
        Material,
        Mesh,
        Shader,
        Texture,
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

    enum class MeshPrimitive {
        Quad,
        Plane,
        Cube,
        Sphere,
    };

    enum class TexturePrimitive {
        Grid,
    };

    class AssetManager final {
    public:
        static Mesh *GetMeshPrimitive(MeshPrimitive mesh_primitive);
        static Texture *GetTexturePrimitive(TexturePrimitive texture_primitive);
        inline static Material *GetDefaultMaterial() { return s_default_material; }
    private:
        ~AssetManager() = delete;
        AssetManager() = delete;
    private:
        static void Initialize();
        static void Shutdown();
    private:
        inline static Mesh *s_mesh_primitive_quad;
        inline static Mesh *s_mesh_primitive_plane;
        inline static Mesh *s_mesh_primitive_cube;
        inline static Mesh *s_mesh_primitive_sphere;

        inline static Texture *s_texture_primitive_grid;

        inline static Material *s_default_material;
    private:
        friend class Engine;
    };
    
}