#pragma once

namespace Hyperion {

    class Timer {
    public:
        virtual float ElapsedSeconds() const = 0;
        virtual float ElapsedMilliSeconds() const = 0;

        virtual void Reset() = 0;

        static Timer *StartNew();
    };

}