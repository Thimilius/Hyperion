#include "hyppch.hpp"

#include "hyperion/core/threading/auto_reset_event.hpp"

namespace Hyperion::Threading {

    void AutoResetEvent::Notify() {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_signalled = true;
        m_condition_variable.notify_one();
    }

    void AutoResetEvent::Wait() {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_condition_variable.wait(lock, [&] { return m_signalled; });
        m_signalled = false;
    }

}