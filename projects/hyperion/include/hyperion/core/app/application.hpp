#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/core/app/events/mouse_events.hpp"
#include "hyperion/core/app/events/gamepad_events.hpp"

namespace Hyperion {

    class Application {
    private:
        Ref<Window> m_window;
        bool m_running;
        ApplicationSettings m_starting_settings;

        inline static Application *s_instance;
    public:
        Application(const ApplicationSettings &settings);
        virtual ~Application() = default;

        inline Ref<Window> GetWindow() const { return m_window; }

        int Run();
        void Exit();

        inline static Application *GetInstance() { return s_instance; }
    protected:
        virtual void OnInit() = 0;
        virtual void OnEvent(Event &event) = 0;
        virtual void OnUpdate(f32 delta_time) = 0;
        virtual void OnRender() = 0;
        virtual void OnTick() = 0;
    private:
        void OnEventInternal(Event &event);

        void OnKeyEvent(KeyEvent &event, bool down);
        void OnMouseButtonEvent(MouseButtonEvent &event, bool down);
        void OnGamepadConnectionChanged(GamepadConnectionChangedEvent &event);
    };

    // This is a function to be defined by the client
    Application *CreateApplication();

}