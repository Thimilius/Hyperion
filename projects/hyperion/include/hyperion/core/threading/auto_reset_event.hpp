#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <atomic>
#include <condition_variable>

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Threading {

    class AutoResetEvent final {
    public:
        AutoResetEvent(bool8 start_value = false);
    public:
        void Notify();
        void Wait();
        bool8 WaitUnblocked();
    private:
        AutoResetEvent(const AutoResetEvent &other) = delete;
        AutoResetEvent &operator=(const AutoResetEvent &other) = delete;
    private:
        std::mutex m_mutex;
        std::condition_variable m_condition_variable;
        std::atomic<bool8> m_signaled = false;
    };

}