//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/asset_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"
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
    void AssetManager::Initialize() {
        ImageLoader::Initialize();
    }

    //--------------------------------------------------------------
    void AssetManager::Shutdown() {
        ImageLoader::Shutdown();
    }

    //--------------------------------------------------------------
    AssetInfo AssetManager::GetNextAssetInfo() {
        return { s_id_counter++, AssetGuid::Create() };
    }

}