//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/threading/critical_section.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Threading {

  class ScopeLock final : public INonCopyable {
  public:
    ScopeLock(const CriticalSection &critical_section) : m_critical_section(&critical_section) { m_critical_section->Lock(); }
    ~ScopeLock() { m_critical_section->Unlock(); }
  private:
    const CriticalSection *m_critical_section;
  };

}
