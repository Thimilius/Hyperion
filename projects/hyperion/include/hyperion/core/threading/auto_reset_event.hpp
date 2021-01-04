#pragma once

#include <condition_variable>

namespace Hyperion::Threading {

    class AutoResetEvent {
    public:
        AutoResetEvent() = default;

        void Notify();
        void Wait();
    private:
        AutoResetEvent(const AutoResetEvent &other) = delete;
        AutoResetEvent &operator=(const AutoResetEvent &other) = delete;

        std::mutex m_mutex;
        std::condition_variable m_condition_variable;
        bool m_signalled = false;
    };

}