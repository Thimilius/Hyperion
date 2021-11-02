//------------------------ Header Guard ------------------------
#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <thread>

//---------------------- Project Includes ----------------------
#include "hyperion/core/threading/thread_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Threading {

    class WindowsThread final {
    public:
        ThreadId GetId();

        void Start(const ThreadStartFunction &start_function);
        void Start(const ParameterizedThreadStartFunction &parameterized_start_function, void *parameter);
        void Join();

        void SetName(const String &name);
    public:
        static uint32 GetSupportedThreadCount();
        static ThreadId GetCurrentThreadId();
        static void Sleep(uint32 milliseconds);
    private:
        std::thread m_thread;
    };

}