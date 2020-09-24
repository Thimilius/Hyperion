#include "hyppch.hpp"

#include "hyperion/platform/windows/windows_timer.hpp"

namespace Hyperion {

    Timer *Timer::Create() {
        return new WindowsTimer();
    }

    WindowsTimer::WindowsTimer() {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        m_frequency = 1.0 / frequency.QuadPart;

        Reset();
    }

    f32 WindowsTimer::ElapsedSeconds() const {
        LARGE_INTEGER current;
        QueryPerformanceCounter(&current);
        u64 cycles = current.QuadPart - m_start.QuadPart;
        return static_cast<f32>(cycles * m_frequency);
    }

    f32 WindowsTimer::ElapsedMilliSeconds() const {
        return ElapsedSeconds() * 1000.0f;
    }

    void WindowsTimer::Reset() {
        QueryPerformanceCounter(&m_start);
    }
}