//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/threading/critical_section.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Threading {

  class ScopeLock final : public INonCopyable {
  public:
    ScopeLock() = default;
    ScopeLock(const CriticalSection &critical_section) : m_critial_section(&critical_section) { m_critial_section->Lock(); }
    ~ScopeLock() { m_critial_section->Unlock(); }
  private:
    const CriticalSection *m_critial_section;
  };

}