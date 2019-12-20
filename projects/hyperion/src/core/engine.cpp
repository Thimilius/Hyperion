#include "hyppch.hpp"

#include "hyperion/core/engine.hpp"

#include "hyperion/app/display.hpp"
#include "hyperion/audio/audio_engine.hpp"
#include "hyperion/rendering/render_command.hpp"

namespace Hyperion {
    
    void Engine::Init() {
        // We initialize the operating system first to get logging ability
        OperatingSystem::GetInstance()->Init();

        HYP_LOG_INFO("Engine", "Initializing...");
        SystemInfo system_info = OperatingSystem::GetInstance()->GetSystemInfo();
        HYP_LOG_INFO("Engine", "Processor Count: {} | Processor Frequency: {:.2f} Ghz | Physical Memory: {:.2f} GB",
            system_info.processor_info.processor_count,
            (float)system_info.processor_info.processor_mhz_frequency / 1000.0f,
            (float)system_info.memory_info.total_physical_memory / (1024.0f * 1024.0f * 1024.0f));

        CDisplay::UpdateDisplayInfos();
        DisplayInfo::DisplayModeInfo mode_info = CDisplay::GetCurrentDisplayModeInfo();
        HYP_LOG_INFO("Engine", "Primary display: {}x{} @{} Hz", mode_info.width, mode_info.height, mode_info.refresh_rate);

        // TEMP: For faster startup time, skip initializing the audio engine
        //Audio::CAudioEngine::Init();

        Rendering::RenderCommand::Init();
    }

    void Engine::Shutdown() {
        Audio::AudioEngine::Shutdown();
    }

}