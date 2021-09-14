//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/engine.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/audio/audio_engine.hpp"
#include "hyperion/core/timer.hpp"
#include "hyperion/core/app/application.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/app/events/app_event.hpp"
#include "hyperion/core/app/events/display_events.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/core/app/events/window_events.hpp"
#include "hyperion/core/memory/memory.hpp"
#include "hyperion/ecs/world/world_manager.hpp"
#include "hyperion/physics/physics_engine.hpp"
#include "hyperion/rendering/render_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Engine::Setup() {
        // We initialize the operating system first to get logging ability.
        OperatingSystem::GetInstance()->Initialize();

        HYP_LOG_INFO("Engine", "Initializing...");
        SystemInfo system_info = OperatingSystem::GetInstance()->GetSystemInfo();
        HYP_LOG_INFO("Engine", "Processor Count: {} | Processor Frequency: {:.2f} Ghz | Physical Memory: {:.2f} GB",
            system_info.processor_info.processor_count,
            static_cast<float32>(system_info.processor_info.processor_mhz_frequency / 1000.0f),
            static_cast<float32>(system_info.memory_info.total_physical_memory / (1024.0f * 1024.0f * 1024.0f)));

        Display::UpdateDisplayInfos();
        DisplayInfo::DisplayModeInfo mode_info = Display::GetCurrentDisplayModeInfo();
        HYP_LOG_INFO("Engine", "Primary display: {}x{} @{} Hz", mode_info.width, mode_info.height, mode_info.refresh_rate);

        EngineLoopSystem &engine_loop = s_settings.core.engine_loop;
        engine_loop.initilization.name = "Initilization";
        engine_loop.initilization.sub_systems = {
            { "MemoryStatsInitilization", MemoryStats::ResetFrameMemory },
            { "TimeInitilization", []() {
                float32 now = Time::s_timer.ElapsedSeconds();
                float32 delta_time = static_cast<float32>(now - Time::s_last_time);
                if (delta_time > Time::GetMaxDeltaTime()) {
                    delta_time = Time::GetMaxDeltaTime();
                }
                Time::s_last_time = now;
                Time::s_accumulator += delta_time;
                Time::s_delta_time = delta_time;
                Time::s_time += delta_time;

                // Accumulate frame times and calculate the average to get more robust frame times and fps.
                Time::s_past_delta_times[Time::s_frame_counter % Time::MAX_PAST_DELTA_TIMES] = delta_time;
                float32 delta_time_average = 0.0f;
                for (uint64 i = 0; i < Time::MAX_PAST_DELTA_TIMES; i++) {
                    delta_time_average += Time::s_past_delta_times[i];
                }
                delta_time_average /= Time::MAX_PAST_DELTA_TIMES;
                Time::s_frame_time = delta_time_average * 1000.0f;
                Time::s_fps = static_cast<uint32>(1.0f / delta_time_average);
                Time::s_frame_counter++;
            } },
            { "InputInitilization", []() { s_application->GetWindow()->Poll(); } }
        };
        engine_loop.fixed_update.name = "FixedUpdate";
        engine_loop.fixed_update.sub_systems = {
            { "ApplicationFixedUpdate", []() { s_application->OnFixedUpdate(Time::GetFixedDeltaTime()); } },
            { "TimeFixedUpdate", []() { Time::s_accumulator -= Time::GetFixedDeltaTime(); } }
        };
        engine_loop.tick.name = "Tick";
        engine_loop.tick.sub_systems = {
            { "ApplicationTick", []() { s_application->OnTick(); } }
        };
        engine_loop.update.name = "Update";
        engine_loop.update.sub_systems = {
            { "ApplicationUpdate", []() { s_application->OnUpdate(Time::GetDeltaTime()); } },
            { "WorldManagerUpdate", []() { WorldManager::Update(); } }
        };
        engine_loop.late_update.name = "LateUpdate";
        engine_loop.late_update.sub_systems = {
            { "AssetManagerLateUpdate", []() { AssetManager::LateUpdate(); } },
            { "RenderEngineLateUpdate", []() { Rendering::RenderEngine::Render(); } }
        };
    }

    //--------------------------------------------------------------
    uint32 Engine::Run() {
        s_running = true;

        s_application = Application::GetInstance();
        s_application->OnSetup(s_settings);

        PreInitialize();

        Initialize();
        s_application->OnInitialize();
        PostInitialize();
        s_application->GetWindow()->Show();

        while (s_running) {
            Iterate();
        }

        s_application->OnShutdown();
        Shutdown();

        return 0;
    }

    //--------------------------------------------------------------
    void Engine::Exit() {
        s_running = false;
    }

    //--------------------------------------------------------------
    void Engine::PreInitialize() {
        HYP_ASSERT_MESSAGE(s_settings.core.max_delta_time > 0, "Max delta time must be greater than zero!");
        Time::s_max_delta_time = s_settings.core.max_delta_time;
        HYP_ASSERT_MESSAGE(s_settings.core.fixed_delta_time > 0, "Fixed delta time must be greater than zero!");
        Time::s_fixed_delta_time = s_settings.core.fixed_delta_time;

        Display::UpdateSize(s_settings.window.width, s_settings.window.height);
        
        Window *window = Window::Create(s_settings.window);
        window->SetAppEventCallback(Engine::OnAppEvent);
        s_application->m_window = window;

        Rendering::RenderEngine::PreInitialize(s_settings.render, window);
    }

    //--------------------------------------------------------------
    void Engine::Initialize() {
        AssetManager::Initialize();

        Audio::AudioEngine::Initialize();
        Rendering::RenderEngine::Initialize();
        Physics::PhysicsEngine::Initialize();

        WorldManager::Initialize();
    }

    //--------------------------------------------------------------
    void Engine::PostInitialize() {

    }

    //--------------------------------------------------------------
    void Engine::Iterate() {
        const EngineLoopSystem &engine_loop = s_settings.core.engine_loop;
        ExecuteEngineLoopSubSystem(engine_loop.initilization);
        while (Time::s_accumulator > Time::GetFixedDeltaTime()) {
            ExecuteEngineLoopSubSystem(engine_loop.fixed_update);
        }
        if (Time::OnInterval(1.0f)) {
            ExecuteEngineLoopSubSystem(engine_loop.tick);
        }
        ExecuteEngineLoopSubSystem(engine_loop.pre_update);
        ExecuteEngineLoopSubSystem(engine_loop.update);
        ExecuteEngineLoopSubSystem(engine_loop.late_update);
    }

    //--------------------------------------------------------------
    void Engine::OnAppEvent(AppEvent &app_event) {
        AppEventDispatcher dispatcher(app_event);

        dispatcher.Dispatch<DisplayChangeAppEvent>([](DisplayChangeAppEvent &display_change_event) {
            Display::UpdateDisplayInfos();
        });

        dispatcher.Dispatch<WindowCloseAppEvent>([](WindowCloseAppEvent &window_close_event) {
            Exit();
        });
        dispatcher.Dispatch<WindowResizeAppEvent>([](WindowResizeAppEvent &window_resize_event) {
            uint32 width = window_resize_event.GetWidth();
            uint32 height = window_resize_event.GetHeight();
            Display::UpdateSize(width, height);
        });

        dispatcher.Dispatch<KeyPressedAppEvent>([](KeyPressedAppEvent &key_pressed_event) {
            if (s_settings.core.allow_altf4) {
                if (key_pressed_event.HasKeyModifier(KeyModifier::Alt) && key_pressed_event.GetKeyCode() == KeyCode::F4) {
                    Exit();
                }
            }
        });
    }

    //--------------------------------------------------------------
    void Engine::Shutdown() {
        // When shutting down we have to be very careful about the order.
        WorldManager::Shutdown();

        Physics::PhysicsEngine::Shutdown();
        Audio::AudioEngine::Shutdown();

        Rendering::RenderEngine::Shutdown();

        delete s_application->GetWindow();
    }

    //--------------------------------------------------------------
    void Engine::ExecuteEngineLoopSubSystem(const EngineLoopSubSystem &engine_loop_sub_system) {
        // We explicitly ignore the update function for systems that contain sub systems.
        if (!engine_loop_sub_system.sub_systems.IsEmpty()) {
            for (const EngineLoopSubSystem &sub_system : engine_loop_sub_system.sub_systems) {
                ExecuteEngineLoopSubSystem(sub_system);
            }
        } else {
            if (engine_loop_sub_system.update_function != nullptr) {
                engine_loop_sub_system.update_function();
            }
        }
    }

    //--------------------------------------------------------------
    void Engine::PanicInternal(const String &title, const String &message) {
        OperatingSystem::GetInstance()->DisplayError(title, message);
        Exit();
    }

}