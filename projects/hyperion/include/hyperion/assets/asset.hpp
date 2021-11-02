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
    protected:
        Asset(AssetInfo info) : m_info(info) { }
        virtual ~Asset() = default;
    public:
        inline const AssetInfo &GetAssetInfo() const { return m_info; }
        inline const AssetResourceInfo &GetResourceInfo() const { return m_resource_info; }
        inline bool8 IsDirty() const { return m_is_dirty; }

        virtual AssetType GetAssetType() const = 0;
    protected:
        bool8 ValidateDataAccess() const;
        void SetDirty();
    private:
        void ResetDirty();
    private:
        AssetInfo m_info;
        AssetResourceInfo m_resource_info;

        bool8 m_is_dirty = false;
    private:
        friend class Hyperion::AssetManager;
        friend class Hyperion::AssetLoadSystem;
    };

}