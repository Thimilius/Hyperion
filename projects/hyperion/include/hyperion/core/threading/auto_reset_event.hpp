#pragma once

#include <condition_variable>

namespace Hyperion::Threading {

    class AutoResetEvent final {
    public:
        AutoResetEvent(bool start_value = false);

        void Notify();
        void Wait();
        bool WaitUnblocked();
    private:
        AutoResetEvent(const AutoResetEvent &other) = delete;
        AutoResetEvent &operator=(const AutoResetEvent &other) = delete;

        std::mutex m_mutex;
        std::condition_variable m_condition_variable;
        bool m_signaled = false;
    };

}