#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/app/window.hpp"

namespace Hyperion {

    class Application {
    private:
        Scope<Window> m_window;
        bool m_running;
        ApplicationSettings m_starting_settings;

        inline static Application *s_instance;
    public:
        inline Window *GetWindow() const { return m_window.get(); }

        void Exit();

        inline static Application *GetInstance() { return s_instance; }
    protected:
        Application(const ApplicationSettings &settings);

        virtual void OnInit() { }
        virtual void OnEvent(Event &event) { }
        virtual void OnUpdate(f32 delta_time) { }
        virtual void OnRender() { }
        virtual void OnTick() { }
    private:
        u32 Run();

        void OnEventInternal(Event &event);

        friend class Main;
    };

    // This is a function to be defined by the client
    Application *CreateApplication();

}