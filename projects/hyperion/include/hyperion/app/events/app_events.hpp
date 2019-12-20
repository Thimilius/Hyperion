#pragma once

#include "hyperion/app/events/event.hpp"

namespace Hyperion::Events {

    class CAppDisplayChangeEvent : public CEvent {
    public:
        EEventType GetType() const override { return EEventType::AppDisplayChange; }
        EEventCategory GetCategory() const override { return EEventCategory::App; }

        static EEventType GetStaticType() { return EEventType::AppDisplayChange; }
    };

}