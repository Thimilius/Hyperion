#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/guid.hpp"

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

        inline static constexpr AssetId INVALID_ID = -1;
    };

}