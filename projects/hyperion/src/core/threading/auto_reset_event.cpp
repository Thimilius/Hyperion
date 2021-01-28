#include "hyppch.hpp"

#include "hyperion/core/threading/auto_reset_event.hpp"

// NOTE: Usually you would use a lock in the Notify and WaitUnblocked functions as well.
// But because in our scenario they are only ever called on one thread that is not really necessary.
// At least I think so...

namespace Hyperion::Threading {

    AutoResetEvent::AutoResetEvent(bool start_value) {
        m_signaled = start_value;
    }

    void AutoResetEvent::Notify() {
        m_signaled = true;
        m_condition_variable.notify_one();
    }

    void AutoResetEvent::Wait() {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_condition_variable.wait(lock, [&]() { return m_signaled.load(); });
        m_signaled = false;
    }

    bool AutoResetEvent::WaitUnblocked() {
        if (m_signaled) {
            m_signaled = false;
            return true;
        } else {
            return false;
        }
    }

}