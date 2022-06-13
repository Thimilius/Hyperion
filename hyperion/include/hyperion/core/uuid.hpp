//------------------------ Header Guard ------------------------
#pragma once

#ifdef HYP_PLATFORM_WINDOWS
#include "hyperion/platform/windows/windows_uuid.hpp"

namespace Hyperion {
  using UUID = WindowsUUID;
}
#else
#error Platform not implemented
#endif

namespace std {

  template<>
  struct std::formatter<Hyperion::UUID> : std::formatter<Hyperion::String> {
    auto format(Hyperion::UUID uuid, std::format_context& ctx) {
      return std::formatter<Hyperion::String>::format(uuid.ToString(), ctx);
    }
  };

  template<>
  struct hash<Hyperion::UUID> {
    std::size_t operator()(const Hyperion::UUID &uuid) const noexcept {
      return (hash<uint64>()(uuid.m_data[0]) ^ (hash<uint64>()(uuid.m_data[1]) << 1)) >> 1;
    }
  };

}
