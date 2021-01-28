#include "hyppch.hpp"

#include "hyperion/core/system/engine.hpp"

#include "hyperion/assets/asset.hpp"
#include "hyperion/audio/audio_engine.hpp"
#include "hyperion/core/timer.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/app/events/app_events.hpp"
#include "hyperion/core/app/events/window_events.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/core/object/object.hpp"
#include "hyperion/core/memory/memory.hpp"
#include "hyperion/core/threading/synchronization.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/physics/physics_engine.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/scripting/scripting_engine.hpp"

namespace Hyperion {

    void Engine::Setup() {
        // We initialize the operating system first to get logging ability
        OperatingSystem::GetInstance()->Init();

        HYP_LOG_INFO("Engine", "Initializing...");
        SystemInfo system_info = OperatingSystem::GetInstance()->GetSystemInfo();
        HYP_LOG_INFO("Engine", "Processor Count: {} | Processor Frequency: {:.2f} Ghz | Physical Memory: {:.2f} GB",
            system_info.processor_info.processor_count,
            static_cast<float32>(system_info.processor_info.processor_mhz_frequency / 1000.0f),
            static_cast<float32>(system_info.memory_info.total_physical_memory / (1024.0f * 1024.0f * 1024.0f)));

        Display::UpdateDisplayInfos();
        DisplayInfo::DisplayModeInfo mode_info = Display::GetCurrentDisplayModeInfo();
        HYP_LOG_INFO("Engine", "Primary display: {}x{} @{} Hz", mode_info.width, mode_info.height, mode_info.refresh_rate);
    }

    void Engine::PreInit() {
        HYP_ASSERT_MESSAGE(s_settings.core.max_delta_time > 0, "Max delta time must be greater than zero!");
        Time::s_max_delta_time = s_settings.core.max_delta_time;
        HYP_ASSERT_MESSAGE(s_settings.core.fixed_delta_time > 0, "Fixed delta time must be greater than zero!");
        Time::s_fixed_delta_time = s_settings.core.fixed_delta_time;
        
        Display::UpdateSize(s_settings.window.width, s_settings.window.height);

        s_application = Application::GetInstance();
        Window *window = Window::Create(s_settings.window);
        window->SetEventCallback(Engine::OnEvent);
        s_application->m_window = window;

        Rendering::RenderEngine::PreInit(s_settings.render, window);
    }

    void Engine::Init() {
        Audio::AudioEngine::Init();
        AssetManager::Init();
        Rendering::RenderEngine::Init();
        Physics::PhysicsEngine::Init();
        ScriptingEngine::Init(s_settings.scripting);
    }

    uint32 Engine::Run() {
        s_running = true;

        PreInit();

        if (s_settings.render.threading_mode == Rendering::RenderThreadingMode::MultiThreaded) {
            Synchronization::NotifyUpdateReady();
            Synchronization::WaitForRenderReady();
        }

        Init();
        s_application->OnInit();
        s_application->GetWindow()->Show();

        s_stats.timer = Timer::Create();
        while (s_running) {
            Iterate();
        }
        delete s_stats.timer;

        s_application->OnShutdown();
        Shutdown();

        return 0;
    }

    void Engine::Iterate() {
        const EngineLoopSystem &engine_loop = s_settings.core.engine_loop;
        ExecuteEngineLoopSubSystem(engine_loop.initilization);
        while (s_stats.accumulator > Time::GetFixedDeltaTime()) {
            ExecuteEngineLoopSubSystem(engine_loop.fixed_update);
        }
        if (Time::OnInterval(1.0f)) {
            ExecuteEngineLoopSubSystem(engine_loop.tick);
        }
        ExecuteEngineLoopSubSystem(engine_loop.pre_update);
        ExecuteEngineLoopSubSystem(engine_loop.update);
        ExecuteEngineLoopSubSystem(engine_loop.late_update);
    }

    void Engine::OnEvent(Event &event) {
        // Events dispatched internally should probably never be set as handled
        EventDispatcher dispatcher(event);

        // Handle app events
        dispatcher.Dispatch<AppDisplayChangeEvent>([](AppDisplayChangeEvent &app_display_change_event) {
            Display::UpdateDisplayInfos();
        });

        // Handle window events
        dispatcher.Dispatch<WindowCloseEvent>([](WindowCloseEvent &window_close_event) {
            Exit();
        });
        dispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent &window_resize_event) {
            uint32 width = window_resize_event.GetWidth();
            uint32 height = window_resize_event.GetHeight();
            Display::UpdateSize(width, height);
        });

        // Handle key events
        dispatcher.Dispatch<KeyPressedEvent>([](KeyPressedEvent &key_pressed_event) {
            if (s_settings.core.allow_altf4) {
                // Explicitly handle alt-f4 for closing
                if (key_pressed_event.HasKeyModifier(KeyModifier::Alt) && key_pressed_event.GetKeyCode() == KeyCode::F4) {
                    Exit();
                }
            }
        });

        // Forward event to client
        Application::GetInstance()->OnEvent(event);
    }

    void Engine::Exit() {
        s_running = false;
        Rendering::RenderEngine::Exit();
    }

    void Engine::Shutdown() {
        WorldManager::Shutdown();
        ObjectManager::Shutdown();
        ScriptingEngine::Shutdown();
        Physics::PhysicsEngine::Shutdown();
        Rendering::RenderEngine::Shutdown();
        AssetManager::Shutdown();
        Audio::AudioEngine::Shutdown();

        delete Application::GetInstance()->GetWindow();
    }

    void Engine::ExecuteEngineLoopSubSystem(const EngineLoopSubSystem &engine_loop_sub_system) {
        // We ignore the update function for systems that contain sub systems
        if (engine_loop_sub_system.sub_systems.size() > 0) {
            for (const EngineLoopSubSystem &sub_system : engine_loop_sub_system.sub_systems) {
                ExecuteEngineLoopSubSystem(sub_system);
            }
        } else {
            if (engine_loop_sub_system.update_function != nullptr) {
                engine_loop_sub_system.update_function();
            }
        }
    }

    void Engine::TimeInitilization() {
        s_stats.frame++;
        s_stats.fps_counter++;
        float32 now = s_stats.timer->ElapsedSeconds();
        float32 delta_time = static_cast<float32>(now - s_stats.last_time);
        if (delta_time > Time::GetMaxDeltaTime()) {
            delta_time = Time::GetMaxDeltaTime();
        }
        s_stats.last_time = now;
        s_stats.accumulator += delta_time;
        Time::s_delta_time = delta_time;
        Time::s_time += delta_time;
    }

    void Engine::InputInitilization() {
        s_application->GetWindow()->Poll();
    }

    void Engine::ApplicationFixedUpdate() {
        s_application->OnFixedUpdate(Time::GetFixedDeltaTime());
    }

    void Engine::TimeFixedUpdate() {
        s_stats.accumulator -= Time::GetFixedDeltaTime();
    }

    void Engine::TimeTick() {
        Time::s_fps = s_stats.fps_counter;
        Time::s_frame_time = 1000.0f / s_stats.fps_counter;
        s_stats.fps_counter = 0;
    }

    void Engine::ApplicationTick() {
        s_application->OnTick();
    }

    void Engine::ApplicationUpdate() {
        s_application->OnUpdate(Time::GetDeltaTime());
    }

    void Engine::WorldManagerUpdate() {
        WorldManager::Update(Time::GetDeltaTime());
    }

    void Engine::PhysicsEngineFixedUpdate() {
        Physics::PhysicsEngine::FixedUpdate(Time::GetFixedDeltaTime());
    }

    void Engine::ObjectManagerLateUpdate() {
        ObjectManager::LateUpdate();
    }

    void Engine::RenderEngineLateUpdate() {
        Rendering::RenderEngine::Render();
    }

    void Engine::PanicInternal(const String &title, const String &message) {
        OperatingSystem::GetInstance()->DisplayError(title, message);
        Exit();
    }

}