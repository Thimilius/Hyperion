#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class AssetManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class AssetType {
        Mesh
    };

    using AssetId = uint64;

    class Asset : public INonCopyable {
    protected:
        Asset(AssetId id) : m_id(id) { }
        virtual ~Asset() = default;
    public:
        inline AssetId GetId() const { return m_id; }

        virtual AssetType GetAssetType() const = 0;
    private:
        AssetId m_id;
    private:
        friend class Hyperion::AssetManager;
    };

}