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
    void Asset::OnClone(Object *clone) {
        Object::OnClone(clone);

        // The resource id does not need to be copied as its distinct for each asset.
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
