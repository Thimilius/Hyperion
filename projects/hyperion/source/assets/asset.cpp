//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/asset.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  bool8 Asset::ValidateDataAccess() const {
    if (m_info.data_access == AssetDataAccess::None) {
      HYP_LOG_ERROR("Asset", "Trying to read/write data on an asset that is not marked for read and write!");
      return false;
    } else {
      return true;
    }
  }

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