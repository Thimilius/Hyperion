#pragma once

#include "hyperion/core/timer.hpp"

#include <Windows.h>

namespace Hyperion {

    class CWindowsTimer : public CTimer {
    private:
        LARGE_INTEGER m_start;
        double m_frequency;
    public:
        CWindowsTimer();

        float ElapsedSeconds() const override;
        float ElapsedMilliSeconds() const override;

        void Reset() override;
    };

}