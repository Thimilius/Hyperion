#include "hyppch.hpp"

#include "hyperion/system/engine.hpp"

namespace Hyperion {

    void CEngine::Init() {
        // We initialize the operating system first to get logging ability
        COperatingSystem::GetInstance()->Init();

        HYP_CORE_INFO("[Engine] - Initializing...");
        SSystemInfo system_info = COperatingSystem::GetInstance()->GetSystemInfo();
        HYP_CORE_INFO("[Engine] - Processor Count: {} | Processor Frequency: {:.2f} Ghz | Physical Memory: {:.2f} GB",
            system_info.processor_info.processor_count,
            (float)system_info.processor_info.processor_mhz_frequency / 1000.0f,
            (float)system_info.memory_info.total_physical_memory / (1024.0f * 1024.0f * 1024.0f));

        CDisplay::UpdateDisplayInfos();
        CDisplayInfo::SDisplayModeInfo mode_info = CDisplay::GetCurrentDisplayModeInfo();
        HYP_CORE_INFO("[Engine] - Primary display: {}x{} @{} Hz", mode_info.width, mode_info.height, mode_info.refresh_rate);

        Rendering::CRenderCommand::Init();
    }

}