#include "hyppch.hpp"

#include "hyperion/app/application.hpp"

namespace Hyperion {

    CApplication *CApplication::s_instance = nullptr;

    CApplication::CApplication() {
        HYP_ASSERT(!s_instance);
        s_instance = this;

        CEngine::Init();

        m_window = CWindow::Create(CString("Hyperion"), Math::SVec2(1280, 720), EWindowMode::Windowed, EVSyncMode::EveryVBlank);
    }

    int CApplication::Run() {
        m_running = true;

        OnInit();

        CTimer *timer = CTimer::StartNew();
        float last_time = 0, tick_timer = 0;
        u64 frame_counter = 0;
        while (m_running) {
            float now = timer->ElapsedSeconds();
            float delta = now - last_time;
            last_time = now;

            tick_timer += delta;

            CTime::s_delta = delta;
            CTime::s_time += delta;

            frame_counter++;
            OnUpdate(delta);

            if (tick_timer > 1.0f) {
                u32 fps = (u32)(frame_counter * (1.0 / tick_timer));
                CTime::s_fps = fps;
                CTime::s_frame_time = 1000.0f / fps;
                OnTick();

                frame_counter = 0;
                tick_timer = 0;
            }

            m_window->Update();
        }

        return 0;
    }

    void CApplication::Exit() {
        m_running = false;
    }

}