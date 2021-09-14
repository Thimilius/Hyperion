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

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Material *AssetManager::CreateMaterial(Shader *shader) {
        Material *material = new Material(GetNextAssetInfo(), shader);
        s_materials.Add(material);
        return material;
    }

    //--------------------------------------------------------------
    Mesh *AssetManager::CreateMesh() {
        Mesh *mesh = new Mesh(GetNextAssetInfo());
        s_meshes.Add(mesh);
        return mesh;
    }

    //--------------------------------------------------------------
    Texture2D *AssetManager::CreateTexture2D() {
        Texture2D *texture = new Texture2D(GetNextAssetInfo());
        s_textures.Add(texture);
        return texture;
    }

    //--------------------------------------------------------------
    Shader *AssetManager::CreateShader(const String &source) {
        Shader *shader = new Shader(GetNextAssetInfo(), source);
        s_shaders.Add(shader);
        return shader;
    }

    //--------------------------------------------------------------
    void AssetManager::Unload(Asset *asset) {
        switch (asset->GetAssetType()) {
            case AssetType::Material: {
                Material *material = static_cast<Material *>(asset);
                HYP_ASSERT(s_materials.Contains(material));
                s_materials.Remove(material);
                break;
            }
            case AssetType::Mesh: {
                Mesh *mesh = static_cast<Mesh *>(asset);
                HYP_ASSERT(s_meshes.Contains(mesh));
                s_meshes.Remove(mesh);
                break;
            }
            case AssetType::Shader: {
                Shader *shader = static_cast<Shader *>(asset);
                HYP_ASSERT(s_shaders.Contains(shader));
                s_shaders.Remove(shader);
                break;
            }
            case AssetType::Texture: {
                Texture *texture = static_cast<Texture *>(asset);
                HYP_ASSERT(s_textures.Contains(texture));
                s_textures.Remove(texture);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        HYP_ASSERT(!s_assets_to_unload.Contains(asset));
        s_assets_to_unload.Add(asset);
    }

    //--------------------------------------------------------------
    void AssetManager::Initialize() {
        ImageLoader::Initialize();
        MeshLoader::Initialize();
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
    void AssetManager::AddDirtyAsset(Asset *asset) {
        HYP_ASSERT(!s_assets_to_load.Contains(asset));
        s_assets_to_load.Add(asset);
    }

    //--------------------------------------------------------------
    AssetInfo AssetManager::GetNextAssetInfo() {
        return { s_id_counter++, AssetGuid::Generate() };
    }

}