#include "hyppch.hpp"

#include "hyperion/platform/windows/windows_timer.hpp"

namespace Hyperion {

    CTimer *CTimer::StartNew() {
        return new CWindowsTimer();
    }

    CWindowsTimer::CWindowsTimer() {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        m_frequency = 1.0 / frequency.QuadPart;

        Reset();
    }

    float CWindowsTimer::ElapsedSeconds() const {
        LARGE_INTEGER current;
        QueryPerformanceCounter(&current);
        u64 cycles = current.QuadPart - m_start.QuadPart;
        return (float)(cycles * m_frequency);
    }

    float CWindowsTimer::ElapsedMilliSeconds() const {
        return ElapsedSeconds() * 1000.0f;
    }

    void CWindowsTimer::Reset() {
        QueryPerformanceCounter(&m_start);
    }
}