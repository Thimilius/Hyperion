#pragma once

#include "hyperion/core/timer.hpp"

#include <Windows.h>

namespace Hyperion {

    class WindowsTimer : public Timer {
    public:
        WindowsTimer();

        f32 ElapsedSeconds() const override;
        f32 ElapsedMilliSeconds() const override;

        void Reset() override;
    private:
        LARGE_INTEGER m_start;
        f64 m_frequency;
    };

}