#include "hyppch.hpp"

#include "hyperion/assets/asset.hpp"

#include "hyperion/assets/font.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/core/io/image_loader.hpp"

namespace Hyperion {

    Asset::Asset() {
        m_resource_id = Resource::GetNextResourceId();
    }

    Mesh *AssetManager::GetMeshPrimitive(MeshPrimitive mesh_primitive) {
        switch (mesh_primitive) {
            case MeshPrimitive::Quad: return s_mesh_primitive_quad;
            case MeshPrimitive::Plane: return s_mesh_primitive_plane;
            case MeshPrimitive::Cube: return s_mesh_primitive_cube;
            case MeshPrimitive::Sphere: return s_mesh_primitive_sphere;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Texture *AssetManager::GetTexturePrimitive(TexturePrimitive texture_primitive) {
        switch (texture_primitive) {
            case TexturePrimitive::Grid: return s_texture_primitive_grid;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    void AssetManager::Initialize() {
        ImageLoader::Initialize();
        FontLoader::Initialize();
        MeshFactory::Initialize();

        s_mesh_primitive_quad = MeshFactory::CreateQuad(1.0f, 1.0f);
        s_mesh_primitive_plane = MeshFactory::CreatePlane(10.0f, 10.0f);
        s_mesh_primitive_cube = MeshFactory::CreateCube(1.0f);
        s_mesh_primitive_sphere = MeshFactory::CreateSphere(1.0f);

        Image *image = ImageLoader::Load("data/textures/checkerboard.png");
        s_texture_primitive_grid = Texture2D::Create(image->GetWidth(), image->GetHeight(), Rendering::TextureFormat::RGB24, Rendering::TextureParameters(), image->GetPixels());
        Object::Destroy(image);
    }

    void AssetManager::Shutdown() {
        MeshFactory::Shutdown();
        FontLoader::Shutdown();
        ImageLoader::Shutdown();
    }

}
