#include "hyppch.hpp"

#include "hyperion/core/engine.hpp"

#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/audio/audio_engine.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/forward_renderer.hpp"
#include "hyperion/rendering/immediate_renderer.hpp"
#include "hyperion/entity/object_manager.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/editor/editor_engine.hpp"
#include "hyperion/physics/physics_engine.hpp"

namespace Hyperion {
    
    void Engine::Setup() {
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

    void Engine::Init(const ApplicationSettings &settings) {
        Rendering::RenderEngine::Init(settings.render);

        AssetManager::Init(settings.assets);

        // TODO: This should be moved into render engine but cant because we need the assets
        Rendering::ForwardRenderer::Init();
        Rendering::ImmediateRenderer::Init();

        Rendering::Font::Init();

        Physics::PhysicsEngine::Init(settings.physics);
        Audio::AudioEngine::Init(settings.audio);

        WorldManager::Init(settings.entity);

        Editor::EditorEngine::Init();
    }

    void Engine::Update(f32 delta_time) {
        AssetManager::Update();
        Physics::PhysicsEngine::Update(delta_time);
        ObjectManager::Update(delta_time);
        Editor::EditorEngine::Update(delta_time);
    }

    void Engine::LateUpdate() {
        ObjectManager::LateUpdate();
    }

    void Engine::Render() {
        Rendering::RenderEngine::Render();
    }

    void Engine::Tick() {
        Editor::EditorEngine::Tick();
    }

    void Engine::Shutdown() {
        WorldManager::Shutdown();
        AssetManager::Shutdown();
        Audio::AudioEngine::Shutdown();
        Physics::PhysicsEngine::Shutdown();
        Rendering::RenderEngine::Shutdown();
    }

}