#include "hyppch.hpp"

#include "hyperion/core/app/application.hpp"

#include "hyperion/core/timer.hpp"
#include "hyperion/core/engine.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/app/display.hpp"

namespace Hyperion {

    Application *Application::s_instance = nullptr;

    Application::Application(const ApplicationSettings &settings) {
        HYP_ASSERT_MESSAGE(!s_instance, "Trying to create application more than once!");
        s_instance = this;
        m_starting_settings = settings;

        Engine::Init(settings);
        
        HYP_ASSERT_MESSAGE(settings.app.max_delta_time > 0, "Max delta time must be greater than zero!");
        Time::s_max_delta_time = settings.app.max_delta_time;

        m_window = Window::Create(settings.window.title, settings.window.width, settings.window.height, settings.window.window_mode, settings.window.vsync_mode, settings.renderer.backend);
        m_window->SetEventCallbackFunction(std::bind(&Application::OnEventInternal, this, std::placeholders::_1));
    }

    int Application::Run() {
        m_running = true;

        Engine::Setup(m_starting_settings);
        OnInit();
        m_window->Show();

        Ref<Timer> timer = Timer::Create();
        f64 last_time = 0, tick_timer = 0, accumulated_time = 0;
        u64 frame_counter = 0;
        while (m_running) {
            f32 now = timer->ElapsedSeconds();
            f32 delta_time = (f32)(now - last_time);
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
            OnRender();

            if (tick_timer > 1.0f) {
                u32 fps = (u32)(frame_counter * (1.0 / tick_timer));
                Time::s_fps = fps;
                Time::s_frame_time = 1000.0 / fps;
                OnTick();

                frame_counter = 0;
                tick_timer = 0;
            }

            Input::Update();
            m_window->Update();
        }

        Engine::Shutdown();

        return 0;
    }

    void Application::Exit() {
        m_running = false;
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
            Exit();
        });
        dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent &window_resize_event) {
            Rendering::RenderEngine::SetViewport(0, 0, window_resize_event.GetWidth(), window_resize_event.GetHeight());
        });
        auto window_reset = Input::Reset;
        dispatcher.Dispatch<WindowFocusEvent>([window_reset](WindowFocusEvent &window_focus_event){
            window_reset();
        });

        // Handle key events
        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent &key_pressed_event) {
            // Explicitly handle alt-f4 for closing
            if (key_pressed_event.GetKeyCode() == KeyCode::F4 && key_pressed_event.HasKeyModifier(KeyModifier::Alt)) {
                Exit();
            }

            OnKeyEvent(key_pressed_event, true);
        });
        dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent &key_released_event) {
            OnKeyEvent(key_released_event, false);
        });

        // Handle mouse events
        float &mouse_scroll = Input::s_mouse_scroll;
        dispatcher.Dispatch<MouseScrolledEvent>([&mouse_scroll](MouseScrolledEvent &mouse_scrolled_event) {
            mouse_scroll = mouse_scrolled_event.GetScroll();
        });
        Vec2 &s_mouse_position = Input::s_mouse_position;
        dispatcher.Dispatch<MouseMovedEvent>([&s_mouse_position](MouseMovedEvent &mouse_moved_event) {
            s_mouse_position = Vec2(mouse_moved_event.GetX(), mouse_moved_event.GetY());
        });
        dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent &mouse_button_pressed_event) {
            OnMouseButtonEvent(mouse_button_pressed_event, true);
        });
        dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent &mouse_button_released_event) {
            OnMouseButtonEvent(mouse_button_released_event, false);
        });

        OnEvent(event);
    }

    void Application::OnKeyEvent(KeyEvent &event, bool down) {
        s32 key_code = (s32)event.GetKeyCode();
        Input::s_keys_down[key_code] = !Input::s_keys_last[key_code] && down;
        Input::s_keys[key_code] = down;
        Input::s_keys_up[key_code] = Input::s_keys_last[key_code] && !down;
    }

    void Application::OnMouseButtonEvent(MouseButtonEvent &event, bool down) {
        s32 mouse_button_code = (s32)event.GetMouseButtonCode();
        Input::s_mouse_buttons_down[mouse_button_code] = !Input::s_mouse_buttons_last[mouse_button_code] && down;
        Input::s_mouse_buttons[mouse_button_code] = down;
        Input::s_mouse_buttons_up[mouse_button_code] = Input::s_mouse_buttons_last[mouse_button_code] && !down;
    }

}