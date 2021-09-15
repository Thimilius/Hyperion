//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/asset.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Asset::SetDirty() {
        if (!m_is_dirty) {
            m_is_dirty = true;

            AssetManager::AddDirtyAsset(this);
        }
    }

    //--------------------------------------------------------------
    void Asset::ResetDirty() {
        m_is_dirty = false;
    }

}