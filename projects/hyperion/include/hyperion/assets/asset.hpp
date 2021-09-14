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

    struct AssetInfo {
        AssetId id;
        AssetGuid guid;
    };

    class Asset : public INonCopyable {
    protected:
        Asset(AssetInfo info) : m_info(info) { }
        virtual ~Asset() = default;
    public:
        inline const AssetInfo &GetAssetInfo() const { return m_info; }
        inline bool8 IsDirty() const { return m_is_dirty; }
        inline const Threading::CriticalSection &GetLocker() const { return m_locker; }

        virtual AssetType GetAssetType() const = 0;
    protected:
        void SetDirty();
    private:
        void ResetDirty();
    private:
        AssetInfo m_info;

        bool8 m_is_dirty;

        Threading::CriticalSection m_locker;
    private:
        friend class Hyperion::AssetManager;
        friend class Hyperion::AssetLoadSystem;
    };

}