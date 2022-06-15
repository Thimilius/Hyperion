//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/uuid.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  enum class AssetType {
    None,
    
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

  struct AssetMetadata {
    AssetHandle handle;
    AssetType type = AssetType::None;

    AssetDataAccess data_access;
    
    String file_path;
    bool8 is_loaded = false;
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
