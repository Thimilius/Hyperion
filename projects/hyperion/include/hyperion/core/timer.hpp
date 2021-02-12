#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Timer {
    public:
        virtual ~Timer() = default;
    public:
        virtual float32 ElapsedSeconds() const = 0;
        virtual float32 ElapsedMilliSeconds() const = 0;

        virtual void Reset() = 0;
    public:
        static Timer *Create();
    };

}