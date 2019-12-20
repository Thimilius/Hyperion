#pragma once

#include "hyperion/core/timer.hpp"

#include <Windows.h>

namespace Hyperion {

    class WindowsTimer : public Timer {
    private:
        LARGE_INTEGER m_start;
        double m_frequency;
    public:
        WindowsTimer();

        float ElapsedSeconds() const override;
        float ElapsedMilliSeconds() const override;

        void Reset() override;
    };

}