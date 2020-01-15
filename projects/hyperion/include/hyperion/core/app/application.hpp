#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/app/application_layer.hpp"
#include "hyperion/core/app/window.hpp"

namespace Hyperion {

    class Application {
    private:
        Ref<Window> m_window;
        bool m_running;
        ApplicationSettings m_starting_settings;
        Vector<ApplicationLayer*> m_layers;

        inline static Application *s_instance;
    public:
        Application(const ApplicationSettings &settings);
        virtual ~Application();

        inline Window *GetWindow() const { return m_window.get(); }

        void PushLayer(ApplicationLayer *layer);
        void PopLayer();

        int Run(); // TODO: Make this method access private
        void Exit();

        inline static Application *GetInstance() { return s_instance; }
    protected:
        virtual void OnInit() { }
        virtual void OnEvent(Event &event) { }
        virtual void OnUpdate(f32 delta_time) { }
        virtual void OnRender() { }
        virtual void OnTick() { }
    private:
        void OnEventInternal(Event &event);
    };

    // This is a function to be defined by the client
    Application *CreateApplication();

}