#include "hyppch.hpp"

#include "hyperion/core/engine.hpp"

#include "hyperion/core/app/display.hpp"
#include "hyperion/assets/asset_library.hpp"
#include "hyperion/audio/audio_engine.hpp"
#include "hyperion/rendering/renderer.hpp"
#include "hyperion/rendering/immediate_renderer.hpp"

namespace Hyperion {
    
    void Engine::Init(const ApplicationSettings &settings) {
        // We initialize the operating system first to get logging ability
        OperatingSystem::GetInstance()->Init();

        HYP_LOG_INFO("Engine", "Initializing...");
        SystemInfo system_info = OperatingSystem::GetInstance()->GetSystemInfo();
        HYP_LOG_INFO("Engine", "Processor Count: {} | Processor Frequency: {:.2f} Ghz | Physical Memory: {:.2f} GB",
            system_info.processor_info.processor_count,
            (f32)system_info.processor_info.processor_mhz_frequency / 1000.0f,
            (f32)system_info.memory_info.total_physical_memory / (1024.0f * 1024.0f * 1024.0f));

        Display::UpdateDisplayInfos();
        DisplayInfo::DisplayModeInfo mode_info = Display::GetCurrentDisplayModeInfo();
        HYP_LOG_INFO("Engine", "Primary display: {}x{} @{} Hz", mode_info.width, mode_info.height, mode_info.refresh_rate);
    }

    void Engine::Setup(const ApplicationSettings &settings) {
        Rendering::RenderEngine::Init(settings.renderer.backend);
        Audio::AudioEngine::Init(settings.audio.backend);

        AssetLibrary::Init(settings.assets.hot_loading, settings.assets.shader_path, settings.assets.texture_path);

        Rendering::Renderer::Init();
        Rendering::ImmediateRenderer::Init();
    }

    void Engine::Update() {
        AssetLibrary::Update();
    }

    void Engine::PostUpdate() {
        
    }

    void Engine::Shutdown() {
        AssetLibrary::Shutdown();

        Audio::AudioEngine::Shutdown();
        Rendering::RenderEngine::Shutdown();
    }

}