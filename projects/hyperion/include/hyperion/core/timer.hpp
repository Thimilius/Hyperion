#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class Timer {
    public:
        virtual f32 ElapsedSeconds() const = 0;
        virtual f32 ElapsedMilliSeconds() const = 0;

        virtual void Reset() = 0;

        static Timer *StartNew();
    };

}