#pragma once

#include <thread>

#include "hyperion/common.hpp"

namespace Hyperion::Threading {

    using ThreadId = u32;
    using ThreadStartFunction = std::function<void()>;
    using ParameterizedThreadStartFunction = std::function<void(void *)>;

    class Thread final {
    public:
        ThreadId GetId();

        void Start(const ThreadStartFunction &start_function);
        void Start(const ParameterizedThreadStartFunction &parameterized_start_function, void *parameter);
        void Join();

        void SetName(const String &name);

        static u32 GetSupportedThreadCount();
        static ThreadId GetCurrentThreadId();
        static void Sleep(u32 milliseconds);
    private:
        std::thread m_thread;
    };

}