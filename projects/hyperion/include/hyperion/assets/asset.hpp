#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/guid.hpp"
#include "hyperion/core/threading/critical_section.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class AssetLoadSystem;
    class AssetManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class AssetType {
        Material,
        Mesh,
        Shader,
        Texture
    };

    using AssetId = uint64;
    using AssetGuid = Guid;

    enum class AssetDataAccess {
        None,
        ReadAndWrite
    };

    struct AssetInfo {
        AssetId id;
        AssetGuid guid;

        AssetDataAccess data_access;
    };

    class Asset : public INonCopyable {
    protected:
        Asset(AssetInfo info) : m_info(info) { }
        virtual ~Asset() = default;
    public:
        inline const AssetInfo &GetAssetInfo() const { return m_info; }
        inline bool8 IsDirty() const { return m_is_dirty; }

        virtual AssetType GetAssetType() const = 0;
    protected:
        bool ValidateDataAccess() const;
        void SetDirty();
    private:
        void ResetDirty();
    private:
        AssetInfo m_info;

        bool8 m_is_dirty = false;

        Threading::CriticalSection m_locker;
    private:
        friend class Hyperion::AssetManager;
        friend class Hyperion::AssetLoadSystem;
    };

}