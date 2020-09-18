#include "hyppch.hpp"

#include "hyperion/core/engine.hpp"

#include "hyperion/core/reflection.hpp"
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/audio/audio_engine.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/entity/object_manager.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/editor/editor_engine.hpp"
#include "hyperion/physics/physics_engine.hpp"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Hyperion {
    
    void Engine::Setup() {
        // We initialize the operating system first to get logging ability
        OperatingSystem::GetInstance()->Init();

        RegisterEngineTypes();

        HYP_LOG_INFO("Engine", "Initializing...");
        SystemInfo system_info = OperatingSystem::GetInstance()->GetSystemInfo();
        HYP_LOG_INFO("Engine", "Processor Count: {} | Processor Frequency: {:.2f} Ghz | Physical Memory: {:.2f} GB",
            system_info.processor_info.processor_count,
            static_cast<f32>(system_info.processor_info.processor_mhz_frequency / 1000.0f),
            static_cast<f32>(system_info.memory_info.total_physical_memory / (1024.0f * 1024.0f * 1024.0f)));

        Display::UpdateDisplayInfos();
        DisplayInfo::DisplayModeInfo mode_info = Display::GetCurrentDisplayModeInfo();
        HYP_LOG_INFO("Engine", "Primary display: {}x{} @{} Hz", mode_info.width, mode_info.height, mode_info.refresh_rate);

        RegisterApplicationTypes();
    }

    void Engine::PreInit(const ApplicationSettings &settings) {
        Rendering::RenderEngine::PreInit(settings.render);
    }

    void Engine::Init(const ApplicationSettings &settings) {
        AssetManager::Init(settings.assets);
        Rendering::RenderEngine::Init(settings.render);
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
        Audio::AudioEngine::Shutdown();
        Physics::PhysicsEngine::Shutdown();
        Rendering::RenderEngine::Shutdown();
        AssetManager::Shutdown();
    }

}