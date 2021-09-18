//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/asset_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/shader.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/assets/loader/image_loader.hpp"
#include "hyperion/assets/loader/mesh_loader.hpp"
#include "hyperion/assets/utilities/mesh_generator.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Texture2D *AssetManager::CreateTexture2D() {
        AssetInfo info = GetNextAssetInfo(AssetDataAccess::ReadAndWrite);
        Texture2D *texture = new Texture2D(info);
        s_textures.Insert(info.guid, texture);
        return texture;
    }

    //--------------------------------------------------------------
    Shader *AssetManager::CreateShader(const String &source) {
        AssetInfo info = GetNextAssetInfo(AssetDataAccess::ReadAndWrite);
        Shader *shader = new Shader(info, source);
        s_shaders.Insert(info.guid, shader);
        return shader;
    }

    //--------------------------------------------------------------
    Material *AssetManager::CreateMaterial(Shader *shader) {
        AssetInfo info = GetNextAssetInfo(AssetDataAccess::ReadAndWrite);
        Material *material = new Material(info, shader);
        s_materials.Insert(info.guid, material);
        return material;
    }

    //--------------------------------------------------------------
    Mesh *AssetManager::GetMeshPrimitive(MeshPrimitive mesh_primitive) {
        switch (mesh_primitive) {
            case MeshPrimitive::Quad: return s_primitives.mesh_quad;
            case MeshPrimitive::Plane: return s_primitives.mesh_plane;
            case MeshPrimitive::Cube: return s_primitives.mesh_cube;
            case MeshPrimitive::Sphere: return s_primitives.mesh_sphere;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    //--------------------------------------------------------------
    Mesh *AssetManager::CreateMesh() {
        AssetInfo info = GetNextAssetInfo(AssetDataAccess::ReadAndWrite);
        Mesh *mesh = new Mesh(info);
        s_meshes.Insert(info.guid, mesh);
        return mesh;
    }

    Mesh *AssetManager::CreateMesh(const Rendering::MeshData &data, const Rendering::SubMeshes &sub_meshes, AssetDataAccess data_access) {
        AssetInfo info = GetNextAssetInfo(data_access);
        Mesh *mesh = new Mesh(info, data, sub_meshes);
        s_meshes.Insert(info.guid, mesh);
        return mesh;
    }

    //--------------------------------------------------------------
    void AssetManager::Unload(Asset *asset) {
        AssetGuid asset_guid = asset->GetAssetInfo().guid;
        switch (asset->GetAssetType()) {
            case AssetType::Material: {
                Material *material = static_cast<Material *>(asset);
                HYP_ASSERT(s_materials.Contains(asset_guid));
                s_materials.Remove(asset_guid);
                break;
            }
            case AssetType::Mesh: {
                Mesh *mesh = static_cast<Mesh *>(asset);
                HYP_ASSERT(s_meshes.Contains(asset_guid));
                s_meshes.Remove(asset_guid);
                break;
            }
            case AssetType::Shader: {
                Shader *shader = static_cast<Shader *>(asset);
                HYP_ASSERT(s_shaders.Contains(asset_guid));
                s_shaders.Remove(asset_guid);
                break;
            }
            case AssetType::Texture: {
                Texture *texture = static_cast<Texture *>(asset);
                HYP_ASSERT(s_textures.Contains(asset_guid));
                s_textures.Remove(asset_guid);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        // FIXME: Check for duplicates.
        s_assets_to_unload.Add(asset);
    }

    //--------------------------------------------------------------
    void AssetManager::Initialize() {
        ImageLoader::Initialize();
        MeshLoader::Initialize();

        InitializePrimitives();
    }

    //--------------------------------------------------------------
    void AssetManager::LateUpdate() {
        for (Asset *asset : s_assets_to_unload) {
            delete asset;
        }
        s_assets_to_unload.Clear();
    }

    //--------------------------------------------------------------
    void AssetManager::Shutdown() {
        ImageLoader::Shutdown();
        MeshLoader::Shutdown();
    }

    //--------------------------------------------------------------
    void AssetManager::InitializePrimitives() {
        s_primitives.mesh_quad = MeshGenerator::GenerateQuad(1.0f, 1.0f);
        s_primitives.mesh_plane = MeshGenerator::GeneratePlane(10.0f, 10.0f);
        s_primitives.mesh_cube = MeshGenerator::GenerateCube(1.0f);
        s_primitives.mesh_sphere = MeshGenerator::GenerateSphere(0.5f);
    }

    //--------------------------------------------------------------
    void AssetManager::AddDirtyAsset(Asset *asset) {
        s_assets_to_load.Add(asset);
    }

    //--------------------------------------------------------------
    AssetInfo AssetManager::GetNextAssetInfo(AssetDataAccess data_access) {
        return { s_id_counter++, AssetGuid::Generate(), data_access };
    }

}