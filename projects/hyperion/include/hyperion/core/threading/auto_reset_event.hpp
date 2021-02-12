#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <atomic>
#include <condition_variable>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Threading {

    class AutoResetEvent final {
    public:
        AutoResetEvent(bool start_value = false);
    public:
        void Notify();
        void Wait();
        bool WaitUnblocked();
    private:
        AutoResetEvent(const AutoResetEvent &other) = delete;
        AutoResetEvent &operator=(const AutoResetEvent &other) = delete;
    private:
        std::mutex m_mutex;
        std::condition_variable m_condition_variable;
        std::atomic<bool> m_signaled = false;
    };

}