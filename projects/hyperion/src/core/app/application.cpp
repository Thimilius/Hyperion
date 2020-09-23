#include "hyppch.hpp"

#include "hyperion/core/app/application.hpp"

#include "hyperion/core/timer.hpp"
#include "hyperion/core/engine.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/events/app_events.hpp"
#include "hyperion/core/app/events/window_events.hpp"
#include "hyperion/core/app/events/key_events.hpp"

namespace Hyperion {

    Application::Application(const ApplicationSettings &settings) {
        HYP_ASSERT_MESSAGE(!s_instance, "Trying to create application more than once!");
        s_instance = this;
        m_starting_settings = settings;
        
        HYP_ASSERT_MESSAGE(settings.time.max_delta_time > 0, "Max delta time must be greater than zero!");
        Time::s_max_delta_time = settings.time.max_delta_time;

        Display::UpdateSize(settings.window.width, settings.window.height);

        m_window.reset(Window::Create(settings.window, settings.render.backend));
        m_window->SetEventCallback(std::bind(&Application::OnEventInternal, this, std::placeholders::_1));
    }
    
    void Application::Exit() {
        m_running = false;
    }

    u32 Application::Run() {
        m_running = true;

        OnInit();
        m_window->Show();

        Ref<Timer> timer = Timer::Create();
        f64 last_time = 0, tick_timer = 0, accumulated_time = 0;
        u64 frame_counter = 0;
        while (m_running) {
            f32 now = timer->ElapsedSeconds();
            f32 delta_time = static_cast<f32>(now - last_time);
            if (delta_time > Time::GetMaxDeltaTime()) {
                delta_time = Time::GetMaxDeltaTime();
            }
            last_time = now;
            tick_timer += delta_time;
            Time::s_delta_time = delta_time;
            Time::s_time += delta_time;
            
            frame_counter++;

            Engine::Update(delta_time);
            OnUpdate(delta_time);
            Engine::LateUpdate();

            Engine::Render();

            if (tick_timer > 1.0f) {
                u32 fps = static_cast<u32>(frame_counter * (1.0 / tick_timer));
                Time::s_fps = fps;
                Time::s_frame_time = 1000.0 / fps;

                OnTick();

                frame_counter = 0;
                tick_timer = 0;
            }

            m_window->Update();
        }

        Engine::Shutdown();

        return 0;
    }

    void Application::OnEventInternal(Event &event) {
        // Events dispatched internally should probably never be set as handled
        EventDispatcher dispatcher(event);

        // Handle app events
        auto update_display_infos_func = Display::UpdateDisplayInfos;
        dispatcher.Dispatch<AppDisplayChangeEvent>([update_display_infos_func](AppDisplayChangeEvent &app_display_change_event) {
            update_display_infos_func();
        });

        // Handle window events
        dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent &window_close_event) {
            Application::GetInstance()->Exit();
        });
        auto update_current_size = Display::UpdateSize;
        dispatcher.Dispatch<WindowResizeEvent>([update_current_size](WindowResizeEvent &window_resize_event) {
            u32 width = window_resize_event.GetWidth();
            u32 height = window_resize_event.GetHeight();
            update_current_size(width, height);
        });

        // Handle key events
        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent &key_pressed_event) {
            // Explicitly handle alt-f4 for closing
            if (key_pressed_event.GetKeyCode() == KeyCode::F4 && key_pressed_event.HasKeyModifier(KeyModifier::Alt)) {
                Application::GetInstance()->Exit();
            }
        });

        // Forward event to client
        OnEvent(event);
    }

}