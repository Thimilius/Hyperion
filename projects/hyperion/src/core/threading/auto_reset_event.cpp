#include "hyppch.hpp"

#include "hyperion/core/threading/auto_reset_event.hpp"

namespace Hyperion::Threading {

    AutoResetEvent::AutoResetEvent(bool start_value) {
        m_signaled = start_value;
    }

    void AutoResetEvent::Notify() {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_signaled = true;
        m_condition_variable.notify_one();
    }

    void AutoResetEvent::Wait() {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition_variable.wait(lock, [&] { return m_signaled; });
        m_signaled = false;
    }

    bool AutoResetEvent::WaitOne() {
        std::unique_lock<std::mutex> lock(m_mutex);
        bool signaled = m_condition_variable.wait_until(lock, std::chrono::steady_clock::now(), [&] { return m_signaled; });
        if (signaled) {
            m_signaled = false;
        }
        return signaled;
    }

}