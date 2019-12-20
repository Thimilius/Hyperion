#pragma once

#include "hyperion/app/events/event.hpp"

namespace Hyperion {

    class AppDisplayChangeEvent : public Event {
    public:
        EventType GetType() const override { return EventType::AppDisplayChange; }
        EventCategory GetCategory() const override { return EventCategory::App; }

        static EventType GetStaticType() { return EventType::AppDisplayChange; }
    };

}