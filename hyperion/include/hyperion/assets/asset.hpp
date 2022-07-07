//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class AssetLoadSystem;
  class AssetManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class Asset : public INonCopyable {
    HYP_REFLECT(INonCopyable)
  protected:
    Asset(AssetMetadata metadata) : m_metadata(std::move(metadata)) { }
    virtual ~Asset() = default;
  public:
    inline AssetMetadata GetMetadata() const { return m_metadata; }
    inline AssetHandle GetHandle() const { return m_metadata.handle; }
    inline AssetDataAccess GetDataAccess() const { return m_metadata.data_access; }
    inline bool8 IsDirty() const { return m_is_dirty; }

    virtual AssetType GetAssetType() const = 0;
  protected:
    bool8 ValidateDataAccess() const;
    void SetDirty();
  private:
    void ResetDirty();
  private:
    AssetMetadata m_metadata;

    bool8 m_is_dirty = false;
  private:
    friend class Hyperion::AssetManager;
    friend class Hyperion::AssetLoadSystem;
  };

}
