//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/threading/thread.hpp"

//---------------------- Library Includes ----------------------
#ifdef HYP_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace Hyperion::Threading {

    //--------------------------------------------------------------
    ThreadId Thread::GetId() {
#ifdef HYP_PLATFORM_WINDOWS
        return GetThreadId(m_thread.native_handle());
#endif
    }

    //--------------------------------------------------------------
    void Thread::Start(const ThreadStartFunction &start_function) {
        m_thread = std::thread(start_function);
    }

    //--------------------------------------------------------------
    void Thread::Start(const ParameterizedThreadStartFunction &parameterized_start_function, void *parameter) {
        m_thread = std::thread(parameterized_start_function, parameter);
    }

    //--------------------------------------------------------------
    void Thread::Join() {
        m_thread.join();
    }

    //--------------------------------------------------------------
    void Thread::SetName(const String &name) {
#ifdef HYP_PLATFORM_WINDOWS
        WideString name_wide = StringUtils::Utf8ToUtf16(name);
        SetThreadDescription(m_thread.native_handle(), name_wide.data());
#endif
    }

    //--------------------------------------------------------------
    uint32 Thread::GetSupportedThreadCount() {
        return std::thread::hardware_concurrency();
    }

    //--------------------------------------------------------------
    ThreadId Thread::GetCurrentThreadId() {
#ifdef HYP_PLATFORM_WINDOWS
        return ::GetCurrentThreadId();
#endif
    }

    //--------------------------------------------------------------
    void Thread::Sleep(uint32 milliseconds) {
        std::chrono::milliseconds duration(milliseconds);
        std::this_thread::sleep_for(duration);
    }

}