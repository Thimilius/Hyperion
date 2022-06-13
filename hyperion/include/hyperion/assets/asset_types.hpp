//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/UUID.hpp"

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

  using AssetHandleType = UUID;
  
  struct AssetHandle {
    AssetHandleType handle;

    AssetHandle() = default;
    AssetHandle(AssetHandleType handle) : handle(handle) { }

    bool IsValid() const { return handle.IsValid(); }
    
    bool8 operator==(const AssetHandle &other) const { return handle == other.handle; }
    bool8 operator!=(const AssetHandle &other) const { return !(*this == other); }
    
    operator AssetHandleType() const { return handle; }
  };
  
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

namespace std {

  template<>
  struct std::formatter<Hyperion::AssetHandle> : std::formatter<Hyperion::AssetHandleType> {
    auto format(Hyperion::AssetHandle handle, std::format_context& ctx) {
      return std::formatter<Hyperion::AssetHandleType>::format(handle.handle, ctx);
    }
  };

  template<>
  struct hash<Hyperion::AssetHandle> {
    std::size_t operator()(const Hyperion::AssetHandle &handle) const noexcept {
      return hash<Hyperion::AssetHandleType>()(handle.handle);
    }
  };

}
