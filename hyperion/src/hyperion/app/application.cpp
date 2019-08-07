#include "hyppch.hpp"

#include "hyperion/app/application.hpp"

#define SIM_TICKS_PER_SECOND (50)
#define SIM_TIME_STEP (1.0 / (double)SIM_TICKS_PER_SECOND)
#define SIM_MAX_FRAMESKIP (10)

namespace Hyperion {

    CApplication *CApplication::s_instance = nullptr;

    CApplication::CApplication(const CString &title, u32 width, u32 height, EWindowMode mode) {
        HYP_ASSERT(!s_instance);
        s_instance = this;

        CEngine::Init();

        m_window = CWindow::Create(title, width, height, mode);
        m_window->SetEventCallbackFunction(std::bind(&CApplication::OnEventInternal, this, std::placeholders::_1));
    }

    int CApplication::Run() {
        m_running = true;

        OnInit();

        CTimer *timer = CTimer::StartNew();
        double last_time = 0, tick_timer = 0, accumulated_time = 0;
        u64 frame_counter = 0;
        while (m_running) {
            float now = timer->ElapsedSeconds();
            float delta = (float)(now - last_time);
            last_time = now;

            tick_timer += delta;

            CTime::s_delta_time = delta;
            CTime::s_time += delta;
            CTime::s_fixed_delta_time = (float)SIM_TIME_STEP;

            double sim_delta = delta;
            if (sim_delta > SIM_MAX_FRAMESKIP) {
                sim_delta = SIM_MAX_FRAMESKIP;
            }
            accumulated_time += sim_delta;
            while (accumulated_time >= SIM_TIME_STEP) {
                OnFixedUpdate((float)SIM_TIME_STEP);
                accumulated_time -= SIM_TIME_STEP;
            }

            frame_counter++;
            OnUpdate(delta);

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

        return 0;
    }

    void CApplication::Exit() {
        m_running = false;
    }

    void CApplication::OnEventInternal(CEvent &event) {
        // Events dispatched internally should probably never get handled
        CEventDispatcher dispatcher(event);

        // Handle window events
        dispatcher.Dispatch<CWindowCloseEvent>([this](CWindowCloseEvent &window_close_event) { 
            Exit();
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