#pragma once

//---------------------- Library Includes ----------------------
#include <Windows.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/timer.hpp"

//-------------------- Definition Namespace --------------------
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