#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/non_copyable.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Threading {

    class WindowsCriticalSection : INonCopyable {
    public:
        WindowsCriticalSection();
        ~WindowsCriticalSection();
    public:
        void Lock() const;
        void Unlock() const;
    private:
        struct CriticalSection {
            void *Data1[1];
            long Data2[2];
            void *Data3[3];
        };

        mutable CriticalSection m_critical_section;
    };

}