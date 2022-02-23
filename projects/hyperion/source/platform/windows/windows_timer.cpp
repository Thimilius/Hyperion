//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/windows_timer.hpp"

//---------------------- Library Includes ----------------------
#include <Windows.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  WindowsTimer::WindowsTimer() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    m_frequency = 1.0 / frequency.QuadPart;

    Reset();
  }

  //--------------------------------------------------------------
  float32 WindowsTimer::ElapsedSeconds() const {
    LARGE_INTEGER current;
    QueryPerformanceCounter(&current);
    uint64 cycles = current.QuadPart - m_start;
    return static_cast<float32>(cycles * m_frequency);
  }

  //--------------------------------------------------------------
  float32 WindowsTimer::ElapsedMilliSeconds() const {
    return ElapsedSeconds() * 1000.0f;
  }

  //--------------------------------------------------------------
  void WindowsTimer::Reset() {
    LARGE_INTEGER current;
    QueryPerformanceCounter(&current);
    m_start = current.QuadPart;
  }
}