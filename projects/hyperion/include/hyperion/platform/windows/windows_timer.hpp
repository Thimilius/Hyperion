#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class WindowsTimer {
    public:
        WindowsTimer();
    public:
        float32 ElapsedSeconds() const;
        float32 ElapsedMilliSeconds() const;

        void Reset();
    private:
        uint64 m_start;
        float64 m_frequency;
    };

}