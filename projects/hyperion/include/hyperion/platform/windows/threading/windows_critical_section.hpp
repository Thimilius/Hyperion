//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/non_copyable.hpp"
#include "hyperion/platform/windows/windows_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class WindowsCriticalSection : INonCopyable {
  public:
    WindowsCriticalSection();
    ~WindowsCriticalSection();
  public:
    void Lock() const;
    void Unlock() const;
  private:
    mutable Critical_Section m_critical_section;
  };

}
