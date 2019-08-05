#include "hyppch.hpp"

#include "hyperion/app/application.hpp"

namespace Hyperion {

    CApplication *CApplication::s_instance = nullptr;

    CApplication::CApplication() {
        //HYP_ASSERT(!s_instance);
        
        s_instance = this;
    }

    int CApplication::Run() {
        m_running = true;

        OnInit();
        while (m_running) {
            OnUpdate(1.0f / 60.0f);
            OnTick();
        }

        return 0;
    }

    void CApplication::Exit() {
        m_running = false;
    }

}