#pragma once

#include "hyperion/core/app/events/event.hpp"

namespace Hyperion {

    class ApplicationLayer {
    protected:
        virtual void OnAttach() { }
        virtual void OnEvent(Event &event) { }
        virtual void OnUpdate(f32 delta_time) { }
        virtual void OnRender() { }
        virtual void OnTick() { }
        virtual void OnDetach() { }
        
        friend class Application;
    };

}