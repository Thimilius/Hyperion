#include "hyppch.hpp"

#include "hyperion/app/application.hpp"

namespace Hyperion {

    CApplication *CApplication::s_instance = nullptr;

    CApplication::CApplication() {
        HYP_ASSERT(!s_instance);
        s_instance = this;

        CLog::Get()->Init(); 
        
        HYP_CORE_INFO("Initializing Hyperion...");
        SystemInfo system_info = OperatingSystem::Get()->GetSystemInfo();
        HYP_CORE_INFO("[SystemInfo] - Processor Count: %d | Processor Frequency: %.2f Ghz | Physical Memory: %.2f GB",
            system_info.processor_info.processor_count,
            (float)system_info.processor_info.processor_mhz_frequency / 1000.0f,
            (float)system_info.memory_info.total_physical_memory / (1024.0f * 1024.0f * 1024.0f));
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