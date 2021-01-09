#include "hyppch.hpp"

#include "hyperion/core/threading/thread.hpp"

#ifdef HYP_PLATFORM_WINDOWS
#include <Windows.h>
#else
#error
#endif

namespace Hyperion::Threading {

    ThreadId Thread::GetId() {
#if HYP_PLATFORM_WINDOWS
        return GetThreadId(m_thread.native_handle());
#endif
    }

    void Thread::Start(const ThreadStartFunction &start_function) {
        m_thread = std::thread(start_function);
    }

    void Thread::Start(const ParameterizedThreadStartFunction &parameterized_start_function, void *parameter) {
        m_thread = std::thread(parameterized_start_function, parameter);
    }

    void Thread::Join() {
        m_thread.join();
    }

    u32 Thread::GetSupportedThreadCount() {
        return std::thread::hardware_concurrency();
    }

    ThreadId Thread::GetCurrentThreadId() {
#if HYP_PLATFORM_WINDOWS
        return ::GetCurrentThreadId();
#endif
    }

    void Thread::Sleep(u32 milliseconds) {
        std::chrono::milliseconds duration(milliseconds);
        std::this_thread::sleep_for(duration);
    }

}