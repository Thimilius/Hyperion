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
        // Handle closing explicitly
        if (event.GetType() == EEventType::WindowClose) {
            Exit();
        }

        OnEvent(event);
    }

}