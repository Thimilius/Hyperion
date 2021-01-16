#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class Timer {
    public:
        virtual ~Timer() = default;

        virtual float32 ElapsedSeconds() const = 0;
        virtual float32 ElapsedMilliSeconds() const = 0;

        virtual void Reset() = 0;

        static Timer *Create();
    };

}