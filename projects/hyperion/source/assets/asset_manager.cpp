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
#include "hyperion/core/io/file_system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Texture2D *AssetManager::CreateTexture2D(const Rendering::Texture2DParameters &parameters) {
        AssetInfo info = GetNextAssetInfo(AssetDataAccess::ReadAndWrite);
        Texture2D *texture = new Texture2D(info, parameters);
        s_textures.Insert(info.guid, texture);
        return texture;
    }

    //--------------------------------------------------------------
    Texture2D *AssetManager::CreateTexture2D(const Rendering::Texture2DParameters &parameters, const Rendering::TexturePixelData &pixels, AssetDataAccess data_access) {
        AssetInfo info = GetNextAssetInfo(data_access);
        Texture2D *texture = new Texture2D(info, parameters, pixels);
        s_textures.Insert(info.guid, texture);
        return texture;
    }

    //--------------------------------------------------------------
    Shader *AssetManager::GetShaderPrimitive(ShaderPrimitive shader_primitive) {
        switch (shader_primitive) {
            case ShaderPrimitive::Standard: return s_primitives.shader_standard;
            case ShaderPrimitive::Unlit: return s_primitives.shader_unlit;
            case ShaderPrimitive::Gizmo: return s_primitives.shader_gizmo;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    //--------------------------------------------------------------
    Shader *AssetManager::GetShaderByGuid(AssetGuid guid) {
        auto it = s_shaders.Find(guid);
        if (it == s_shaders.end()) {
            HYP_LOG_ERROR("Asset", "The shader with guid {} does not exist!", guid.ToString());
            return nullptr;
        } else {
            return it->second;
        }
    }

    //--------------------------------------------------------------
    Shader *AssetManager::CreateShader(const String &source) {
        AssetInfo info = GetNextAssetInfo(AssetDataAccess::ReadAndWrite);
        Shader *shader = new Shader(info, source);
        s_shaders.Insert(info.guid, shader);
        return shader;
    }

    //--------------------------------------------------------------
    Material *AssetManager::GetMaterialPrimitive(MaterialPrimitive material_primitive) {
        switch (material_primitive) {
            case MaterialPrimitive::Default: return s_primitives.material_default;
            case MaterialPrimitive::Unlit: return s_primitives.material_unlit;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    //--------------------------------------------------------------
    Material *AssetManager::GetMaterialByGuid(AssetGuid guid) {
        auto it = s_materials.Find(guid);
        if (it == s_materials.end()) {
            HYP_LOG_ERROR("Asset", "The material with guid {} does not exist!", guid.ToString());
            return nullptr;
        } else {
            return it->second;
        }
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
    Mesh *AssetManager::GetMeshByGuid(AssetGuid guid) {
        auto it = s_meshes.Find(guid);
        if (it == s_meshes.end()) {
            HYP_LOG_ERROR("Asset", "The mesh with guid {} does not exist!", guid.ToString());
            return nullptr;
        } else {
            return it->second;
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
        s_primitives.shader_standard = CreateShader(FileSystem::ReadAllText("data/shaders/standard.shader"));
        SetNewGuid(s_primitives.shader_standard, "{6AFEA19E-547B-41F5-A008-4473AE771E06}");
        s_primitives.shader_unlit = CreateShader(FileSystem::ReadAllText("data/shaders/unlit.shader"));
        SetNewGuid(s_primitives.shader_unlit, "{23AA53FE-6A47-4571-BC47-00EAAFA2F54B}");
        s_primitives.shader_gizmo = CreateShader(FileSystem::ReadAllText("data/shaders/gizmo.shader"));
        SetNewGuid(s_primitives.shader_gizmo, "{F05F02F1-A7E1-42B7-9618-F13AB38BCA87}");

        s_primitives.material_default = CreateMaterial(s_primitives.shader_standard);
        SetNewGuid(s_primitives.material_default, "{B2463C27-7FD8-44A2-BC53-2AD74FAA7979}");
        s_primitives.material_unlit = CreateMaterial(s_primitives.shader_unlit);
        SetNewGuid(s_primitives.material_unlit, "{C718D97E-A0D9-4567-AFE6-F264B8C29730}");

        s_primitives.mesh_quad = MeshGenerator::GenerateQuad(1.0f, 1.0f);
        SetNewGuid(s_primitives.mesh_quad, "{D54B554E-2BED-4F36-AF12-9C20C83F4EFB}");
        s_primitives.mesh_plane = MeshGenerator::GeneratePlane(10.0f, 10.0f);
        SetNewGuid(s_primitives.mesh_plane, "{F5464C26-BA78-418D-8DFF-CC67A189DE47}");
        s_primitives.mesh_cube = MeshGenerator::GenerateCube(1.0f);
        SetNewGuid(s_primitives.mesh_cube, "{36E92468-41BB-4B06-918B-958ED7F5DD43}");
        s_primitives.mesh_sphere = MeshGenerator::GenerateSphere(0.5f);
        SetNewGuid(s_primitives.mesh_sphere, "{93DFBF96-D7DB-40B7-91C3-89C6FB1B1E49}");
    }

    //--------------------------------------------------------------
    void AssetManager::SetNewGuid(Asset *asset, const String &guid) {
        AssetGuid old_guid = asset->GetAssetInfo().guid;
        AssetGuid new_guid = AssetGuid::Generate(guid);

        switch (asset->GetAssetType()) {
            case AssetType::Material: {
                Material *material = static_cast<Material *>(asset);
                HYP_ASSERT(s_materials.Contains(old_guid));
                s_materials.Remove(old_guid);
                s_materials.Insert(new_guid, material);
                break;
            }
            case AssetType::Mesh: {
                Mesh *mesh = static_cast<Mesh *>(asset);
                HYP_ASSERT(s_meshes.Contains(old_guid));
                s_meshes.Remove(old_guid);
                s_meshes.Insert(new_guid, mesh);
                break;
            }
            case AssetType::Shader: {
                Shader *shader = static_cast<Shader *>(asset);
                HYP_ASSERT(s_shaders.Contains(old_guid));
                s_shaders.Remove(old_guid);
                s_shaders.Insert(new_guid, shader);
                break;
            }
            case AssetType::Texture: {
                Texture *texture = static_cast<Texture *>(asset);
                HYP_ASSERT(s_textures.Contains(old_guid));
                s_textures.Remove(old_guid);
                s_textures.Insert(new_guid, texture);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        asset->m_info.guid = new_guid;
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