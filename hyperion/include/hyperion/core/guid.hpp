//------------------------ Header Guard ------------------------
#pragma once

#ifdef HYP_PLATFORM_WINDOWS
#include "hyperion/platform/windows/windows_guid.hpp"

namespace Hyperion {
  using Guid = WindowsGuid;
}
#else
#error Platform not implemented
#endif

namespace std {

  template<>
  struct hash<Hyperion::Guid> {
    std::size_t operator()(const Hyperion::Guid &guid) const {
      return (hash<uint64>()(guid.m_data[0]) ^ (hash<uint64>()(guid.m_data[1]) << 1)) >> 1;
    }
  };

}
