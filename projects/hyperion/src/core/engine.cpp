#include "hyppch.hpp"

#include "hyperion/core/engine.hpp"

#include "hyperion/core/app/display.hpp"
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/audio/audio_engine.hpp"
#include "hyperion/rendering/forward_render_pipeline.hpp"
#include "hyperion/rendering/forward_renderer.hpp"
#include "hyperion/rendering/immediate_renderer.hpp"
#include "hyperion/entity/object_manager.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/editor/editor_engine.hpp"

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
        Rendering::RenderEngine::Init(settings.render.backend);

        AssetManager::Init(settings.assets);

        Rendering::ForwardRenderer::Init();
        Rendering::ImmediateRenderer::Init();
        s_render_pipeline.reset(GetRenderPipeline(settings.render));

        Rendering::Font::Init();

        Audio::AudioEngine::Init(settings.audio.backend);

        World *world = settings.entity.start_world;
        if (!world) {
            // NOTE: We are currently forcing a new world with a camera
            world = WorldManager::CreateWorld();
            Entity::Create("Camera", Vec3(), Quaternion::Identity(), nullptr, world)->AddComponent<Camera>();
        }
        WorldManager::SetActiveWorld(world);

        Editor::EditorEngine::Init();
    }

    void Engine::Update(f32 delta_time) {
        AssetManager::Update();
        Editor::EditorEngine::Update(delta_time);

        ObjectManager::Update(delta_time);
    }

    void Engine::LateUpdate() {
        ObjectManager::LateUpdate();
    }

    void Engine::Render() {
        s_render_pipeline->Render();
    }

    void Engine::Tick() {
        Editor::EditorEngine::Tick();
    }

    void Engine::Shutdown() {
        AssetManager::Shutdown();

        Audio::AudioEngine::Shutdown();
        Rendering::RenderEngine::Shutdown();
    }

    Rendering::RenderPipeline *Engine::GetRenderPipeline(const RenderSettings &settings) {
        switch (settings.path) {
            case Rendering::RenderPath::Forward: return new Rendering::ForwardRenderPipeline();
            case Rendering::RenderPath::Custom: {
                HYP_ASSERT_MESSAGE(settings.custom_pipeline, "When using a custom render path, a custom render pipeline must be provided!");
                return settings.custom_pipeline;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}