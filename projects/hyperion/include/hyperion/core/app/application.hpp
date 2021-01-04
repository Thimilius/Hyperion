#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/app/window.hpp"

namespace Hyperion {
    class Main;
}

namespace Hyperion {

    class Application {
    public:
        virtual ~Application();

        inline Window *GetWindow() const { return m_window; }

        void Exit();

        inline static Application *GetInstance() { return s_instance; }
    protected:
        Application(const ApplicationSettings &settings);

        virtual void OnInit() { }
        virtual void OnEvent(Event &event) { }
        virtual void OnUpdate(f32 delta_time) { }
        virtual void OnTick() { }
        virtual void OnShutdown() { }
    private:
        u32 Run();

        void OnEventInternal(Event &event);
    private:
        Window *m_window;
        bool m_running;
        ApplicationSettings m_starting_settings;

        inline static Application *s_instance;
    private:
        friend class Hyperion::Main;
    };

    // This is a function to be defined by the client
    Application *CreateApplication();

}