//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/guid.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  enum class AssetType {
    Font,
    Material,
    Mesh,
    Shader,
    Texture,
    TextureAtlas
  };

  using AssetHandle = Guid;

  enum class AssetDataAccess {
    None,
    Write,
    ReadAndWrite
  };

  // TODO: Replace this with AssetMetadata.
  struct AssetInfo {
    AssetHandle handle;

    AssetDataAccess data_access;
  };

  struct AssetResourceInfo {
    String path;
  };
  
}
