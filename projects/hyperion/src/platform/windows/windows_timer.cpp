#include "hyppch.hpp"

#include "hyperion/platform/windows/windows_timer.hpp"

namespace Hyperion {

    Timer *Timer::StartNew() {
        return new WindowsTimer();
    }

    WindowsTimer::WindowsTimer() {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        m_frequency = 1.0 / frequency.QuadPart;

        Reset();
    }

    float WindowsTimer::ElapsedSeconds() const {
        LARGE_INTEGER current;
        QueryPerformanceCounter(&current);
        u64 cycles = current.QuadPart - m_start.QuadPart;
        return (float)(cycles * m_frequency);
    }

    float WindowsTimer::ElapsedMilliSeconds() const {
        return ElapsedSeconds() * 1000.0f;
    }

    void WindowsTimer::Reset() {
        QueryPerformanceCounter(&m_start);
    }
}