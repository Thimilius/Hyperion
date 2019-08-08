#include "hyppch.hpp"

#include "hyperion/system/engine.hpp"

namespace Hyperion {

    void CEngine::Init() {
        COperatingSystem::GetInstance()->Init();

        HYP_CORE_INFO("[Engine] - Initializing...");
        SSystemInfo system_info = COperatingSystem::GetInstance()->GetSystemInfo();
        HYP_CORE_INFO("[Engine] - Processor Count: %d | Processor Frequency: %.2f Ghz | Physical Memory: %.2f GB",
            system_info.processor_info.processor_count,
            (float)system_info.processor_info.processor_mhz_frequency / 1000.0f,
            (float)system_info.memory_info.total_physical_memory / (1024.0f * 1024.0f * 1024.0f));

        CDisplay::UpdateDisplayInfos();
    }

}