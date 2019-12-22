#pragma once

#include "hyperion/core/app/window.hpp"
#include "hyperion/core/app/events/event.hpp"
#include "hyperion/core/app/events/app_events.hpp"
#include "hyperion/core/app/events/window_events.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/core/app/events/mouse_events.hpp"
#include "hyperion/rendering/render_api.hpp"
#include "hyperion/audio/audio_engine.hpp"

namespace Hyperion {

    struct ApplicationSettings {
        struct {
            f32 max_delta_time = 0.15f;
        } app;

        struct {
            String title = "Hyperion";

            u32 width = 1280;
            u32 height = 720;

            WindowMode window_mode = WindowMode::Windowed;
            VSyncMode vsync_mode = VSyncMode::EveryVBlank;
        } window;

        struct {
            String shader_path = "data/shaders";
        } assets;

        struct {
            Rendering::RenderBackend backend = Rendering::RenderBackend::OpenGL;
        } renderer;

        struct {
            Audio::AudioBackend backend = Audio::AudioBackend::FMod;
        } audio;
    };

    class Application {
    private:
        static Application *s_instance;

        Window *m_window;
        bool m_running;
        ApplicationSettings m_starting_settings;
    public:
        Application(const ApplicationSettings &settings);
        virtual ~Application() = default;

        inline Window *GetWindow() const { return m_window; }

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
    };

    // This is a function to be defined by the client
    Application *CreateApplication();

}