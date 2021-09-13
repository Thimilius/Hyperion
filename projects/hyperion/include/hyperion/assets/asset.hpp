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
        virtual ~Asset();
    public:
        inline const AssetInfo &GetInfo() const { return m_info; }

        virtual AssetType GetAssetType() const = 0;
    private:
        AssetInfo m_info;
    private:
        friend class Hyperion::AssetManager;
    };

}