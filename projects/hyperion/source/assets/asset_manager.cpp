//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/asset_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/assets/loader/image_loader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

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
    void AssetManager::Initialize() {
        ImageLoader::Initialize();
    }

    //--------------------------------------------------------------
    void AssetManager::Shutdown() {
        ImageLoader::Shutdown();
    }

    //--------------------------------------------------------------
    void AssetManager::Unregister(Asset *asset) {
        switch (asset->GetAssetType()) {
            case AssetType::Mesh: {
                Mesh *mesh = static_cast<Mesh *>(asset);
                HYP_ASSERT(s_meshes.Contains(mesh));
                s_meshes.Remove(mesh);
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
    }

    //--------------------------------------------------------------
    AssetInfo AssetManager::GetNextAssetInfo() {
        return { s_id_counter++, AssetGuid::Create() };
    }

}