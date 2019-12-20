#include "hyppch.hpp"

#include "hyperion/app/application.hpp"

#include "hyperion/core/timer.hpp"
#include "hyperion/app/time.hpp"
#include "hyperion/app/display.hpp"
#include "hyperion/app/events/events.hpp"
#include "hyperion/rendering/render_command.hpp"
#include "hyperion/system/engine.hpp"

namespace Hyperion {

    using namespace Events;

    CApplication *CApplication::s_instance = nullptr;

    CApplication::CApplication(const SApplicationSettings &settings) {
        HYP_ASSERT_MESSAGE(!s_instance, "Trying to create application more than once!");
        s_instance = this;

        CEngine::Init();
        
        HYP_ASSERT_MESSAGE(settings.max_delta_time > 0, "Max delta time must be greater than zero!");
        CTime::s_max_delta_time = settings.max_delta_time;

        m_window = CWindow::Create(settings.window.title, settings.window.width, settings.window.height, settings.window.window_mode, settings.window.vsync_mode);
        m_window->SetEventCallbackFunction(std::bind(&CApplication::OnEventInternal, this, std::placeholders::_1));
    }

    int CApplication::Run() {
        m_running = true;

        OnInit();
        m_window->Show();

        CTimer *timer = CTimer::StartNew();
        double last_time = 0, tick_timer = 0, accumulated_time = 0;
        u64 frame_counter = 0;
        while (m_running) {
            float now = timer->ElapsedSeconds();
            float delta_time = (float)(now - last_time);
            if (delta_time > CTime::GetMaxDeltaTime()) {
                delta_time = CTime::GetMaxDeltaTime();
            }

            last_time = now;

            tick_timer += delta_time;

            CTime::s_delta_time = delta_time;
            CTime::s_time += delta_time;

            frame_counter++;
            OnUpdate(delta_time);
            OnRender();

            if (tick_timer > 1.0f) {
                u32 fps = (u32)(frame_counter * (1.0 / tick_timer));
                CTime::s_fps = fps;
                CTime::s_frame_time = 1000.0 / fps;
                OnTick();

                frame_counter = 0;
                tick_timer = 0;
            }

            CInput::Update();
            m_window->Update();
        }

        CEngine::Shutdown();

        return 0;
    }

    void CApplication::Exit() {
        m_running = false;
    }

    void CApplication::OnEventInternal(CEvent &event) {
        // Events dispatched internally should probably never be set as handled
        CEventDispatcher dispatcher(event);

        // Handle app events
        auto update_display_infos_func = CDisplay::UpdateDisplayInfos;
        dispatcher.Dispatch<CAppDisplayChangeEvent>([update_display_infos_func](CAppDisplayChangeEvent &app_display_change_event) {
            update_display_infos_func();
        });

        // Handle window events
        dispatcher.Dispatch<CWindowCloseEvent>([this](CWindowCloseEvent &window_close_event) { 
            Exit();
        });
        dispatcher.Dispatch<CWindowResizeEvent>([this](CWindowResizeEvent &window_resize_event) {
            Rendering::CRenderCommand::SetViewport(0, 0, window_resize_event.GetWidth(), window_resize_event.GetHeight());
        });
        auto window_reset = CInput::Reset;
        dispatcher.Dispatch<CWindowFocusEvent>([window_reset](CWindowFocusEvent &window_focus_event){
            window_reset();
        });

        // Handle key events
        dispatcher.Dispatch<CKeyPressedEvent>([this](CKeyPressedEvent &key_pressed_event) {
            // Explicitly handle alt-f4 for closing
            if (key_pressed_event.GetKeyCode() == EKeyCode::F4 && key_pressed_event.HasKeyModifier(EKeyModifier::Alt)) {
                Exit();
            }

            OnKeyEvent(key_pressed_event, true);
        });
        dispatcher.Dispatch<CKeyReleasedEvent>([this](CKeyReleasedEvent &key_released_event) {
            OnKeyEvent(key_released_event, false);
        });

        // Handle mouse events
        float &mouse_scroll = CInput::s_mouse_scroll;
        dispatcher.Dispatch<CMouseScrolledEvent>([&mouse_scroll](CMouseScrolledEvent &mouse_scrolled_event) {
            mouse_scroll = mouse_scrolled_event.GetScroll();
        });
        Math::SVec2 &s_mouse_position = CInput::s_mouse_position;
        dispatcher.Dispatch<CMouseMovedEvent>([&s_mouse_position](CMouseMovedEvent &mouse_moved_event) {
            s_mouse_position = Math::SVec2(mouse_moved_event.GetX(), mouse_moved_event.GetY());
        });
        dispatcher.Dispatch<CMouseButtonPressedEvent>([this](CMouseButtonPressedEvent &mouse_button_pressed_event) {
            OnMouseButtonEvent(mouse_button_pressed_event, true);
        });
        dispatcher.Dispatch<CMouseButtonReleasedEvent>([this](CMouseButtonReleasedEvent &mouse_button_released_event) {
            OnMouseButtonEvent(mouse_button_released_event, false);
        });

        OnEvent(event);
    }

    void CApplication::OnKeyEvent(CKeyEvent &event, bool down) {
        s32 key_code = (s32)event.GetKeyCode();
        CInput::s_keys_down[key_code] = !CInput::s_keys_last[key_code] && down;
        CInput::s_keys[key_code] = down;
        CInput::s_keys_up[key_code] = CInput::s_keys_last[key_code] && !down;
    }

    void CApplication::OnMouseButtonEvent(CMouseButtonEvent &event, bool down) {
        s32 mouse_button_code = (s32)event.GetMouseButtonCode();
        CInput::s_mouse_buttons_down[mouse_button_code] = !CInput::s_mouse_buttons_last[mouse_button_code] && down;
        CInput::s_mouse_buttons[mouse_button_code] = down;
        CInput::s_mouse_buttons_up[mouse_button_code] = CInput::s_mouse_buttons_last[mouse_button_code] && !down;
    }

}