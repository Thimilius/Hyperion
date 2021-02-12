//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/windows_timer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Timer *Timer::Create() {
        return new WindowsTimer();
    }

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
        uint64 cycles = current.QuadPart - m_start.QuadPart;
        return static_cast<float32>(cycles * m_frequency);
    }

    //--------------------------------------------------------------
    float32 WindowsTimer::ElapsedMilliSeconds() const {
        return ElapsedSeconds() * 1000.0f;
    }

    //--------------------------------------------------------------
    void WindowsTimer::Reset() {
        QueryPerformanceCounter(&m_start);
    }
}