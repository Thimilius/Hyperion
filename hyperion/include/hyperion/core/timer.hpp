#pragma once

namespace Hyperion {

    class CTimer {
    public:
        virtual float ElapsedSeconds() const = 0;
        virtual float ElapsedMilliSeconds() const = 0;

        virtual void Reset() = 0;

        static CTimer *StartNew();
    };

}