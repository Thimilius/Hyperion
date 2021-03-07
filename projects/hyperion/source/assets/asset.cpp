//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/asset.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Asset::Asset() {
        m_resource_id = Resource::GetNextResourceId();
    }

    //--------------------------------------------------------------
    void Asset::OnDestroy() {
        AssetManager::UnregisterAsset(this);
    }

    //--------------------------------------------------------------
    void Asset::RegisterAsset() {
        AssetManager::RegisterAsset(this);
    }

}
