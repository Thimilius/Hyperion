#pragma once

#include <thread>

#include "hyperion/common.hpp"

namespace Hyperion {

    using ThreadId = u32;
    using ThreadStartFunction = std::function<void()>;
    using ParameterizedThreadStartFunction = std::function<void(void *)>;

    class Thread {
    public:
        Thread(const ThreadStartFunction &start_function);
        Thread(const ParameterizedThreadStartFunction &parameterized_start_function, void *parameter);
        virtual ~Thread();

        ThreadId GetId();

        void Join();

        static u32 GetSupportedThreadCount();
        static ThreadId GetCurrentThreadId();
        static void Sleep(u32 milliseconds);
    private:
        std::thread m_thread;
    };

}