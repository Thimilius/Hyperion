//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/threading/windows_critical_section.hpp"

//---------------------- Library Includes ----------------------
#include <Windows.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  WindowsCriticalSection::WindowsCriticalSection() {
    LPCRITICAL_SECTION critical_section = reinterpret_cast<::CRITICAL_SECTION *>(&m_critical_section);
    InitializeCriticalSectionEx(critical_section, 100, CRITICAL_SECTION_NO_DEBUG_INFO);
  }

  //--------------------------------------------------------------
  WindowsCriticalSection::~WindowsCriticalSection() {
    LPCRITICAL_SECTION critical_section = reinterpret_cast<::CRITICAL_SECTION *>(&m_critical_section);
    DeleteCriticalSection(critical_section);
  }

  //--------------------------------------------------------------
  void WindowsCriticalSection::Lock() const {
    LPCRITICAL_SECTION critical_section = reinterpret_cast<::CRITICAL_SECTION *>(&m_critical_section);
    if (TryEnterCriticalSection(critical_section) == 0) {
      EnterCriticalSection(critical_section);
    }
  }

  //--------------------------------------------------------------
  void WindowsCriticalSection::Unlock() const {
    LPCRITICAL_SECTION critical_section = reinterpret_cast<::CRITICAL_SECTION *>(&m_critical_section);
    LeaveCriticalSection(critical_section);
  }

}
