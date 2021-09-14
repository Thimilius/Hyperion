#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/guid.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class AssetManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class AssetType {
        Mesh,
        Texture
    };

    using AssetId = uint64;
    using AssetGuid = Guid;

    struct AssetInfo {
        AssetId id;
        AssetGuid guid;
    };

    class Asset : public INonCopyable {
    protected:
        Asset(AssetInfo info) : m_info(info) { }
        virtual ~Asset() = default;
    public:
        inline const AssetInfo &GetInfo() const { return m_info; }
        inline bool IsDirty() const { return m_is_dirty; }

        virtual AssetType GetAssetType() const = 0;
    protected:
        void SetDirty();
    private:
        void ResetDirty();
    private:
        AssetInfo m_info;

        bool m_is_dirty;
    private:
        friend class Hyperion::AssetManager;
    };

}