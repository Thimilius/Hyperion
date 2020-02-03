#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/app/application_layer.hpp"
#include "hyperion/core/app/window.hpp"

namespace Hyperion {

    namespace Editor {
        class EditorLayer;
    }

    class Application {
    private:
        Scope<Window> m_window;
        bool m_running;
        ApplicationSettings m_starting_settings;

        Scope<Editor::EditorLayer> m_editor_layer;
        Vector<ApplicationLayer*> m_layers;

        inline static Application *s_instance;
    public:
        Application(const ApplicationSettings &settings);
        virtual ~Application();

        inline Window *GetWindow() const { return m_window.get(); }

        inline Editor::EditorLayer *GetEditorLayer() const { return m_editor_layer.get(); }
        void PushLayer(ApplicationLayer *layer);
        void PopLayer();

        void Exit();

        inline static Application *GetInstance() { return s_instance; }
    protected:
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