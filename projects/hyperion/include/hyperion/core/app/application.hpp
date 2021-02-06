#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/app/window.hpp"

namespace Hyperion {
    class Main;
}

namespace Hyperion {

    class Application {
    public:
        virtual ~Application() = default;
    public:
        inline Window *GetWindow() const { return m_window; }

        void Exit();
    public:
        inline static Application *GetInstance() { return s_instance; }
    protected:
        Application(const ApplicationSettings &settings);

        virtual void OnInitialize() { }
        virtual void OnEvent(Event &event) { }
        virtual void OnUpdate(float32 delta_time) { }
        virtual void OnFixedUpdate(float32 delta_time) { }
        virtual void OnTick() { }
        virtual void OnShutdown() { }
    private:
        Window *m_window;
    private:
        inline static Application *s_instance;
    private:
        friend class Hyperion::Engine;
    };

    // This is a function to be defined by the client...
    Application *CreateApplication();

}