#pragma once

#include <Windows.h>

#include "hyperion/core/timer.hpp"

namespace Hyperion {

    class WindowsTimer : public Timer {
    public:
        WindowsTimer();

        float32 ElapsedSeconds() const override;
        float32 ElapsedMilliSeconds() const override;

        void Reset() override;
    private:
        LARGE_INTEGER m_start;
        float64 m_frequency;
    };

}