//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/system/engine.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/audio/audio_engine.hpp"
#include "hyperion/core/timer.hpp"
#include "hyperion/core/app/application.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/app/events/app_events.hpp"
#include "hyperion/core/app/events/window_events.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/core/object/object.hpp"
#include "hyperion/core/memory/memory.hpp"
#include "hyperion/core/profiling/profiling.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/physics/physics_engine.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/scripting/scripting_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Engine::SetMode(EngineMode engine_mode) {
        if (s_engine_mode == engine_mode) {
            return;
        }

        s_engine_mode = engine_mode;
        Scripting::ScriptingEngine::EngineModeChanged(engine_mode);

        // We reset the time after informing the scripting engine.
        // This ensures that we wait before the scripting domain gets properly reloaded.
        Time::s_time_since_engine_mode_change = 0.0f;
    }

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
    }

    //--------------------------------------------------------------
    void Engine::PreInitialize() {
        HYP_ASSERT_MESSAGE(s_settings.core.max_delta_time > 0, "Max delta time must be greater than zero!");
        Time::s_max_delta_time = s_settings.core.max_delta_time;
        HYP_ASSERT_MESSAGE(s_settings.core.fixed_delta_time > 0, "Fixed delta time must be greater than zero!");
        Time::s_fixed_delta_time = s_settings.core.fixed_delta_time;
        
        Display::UpdateSize(s_settings.window.width, s_settings.window.height);

        
        Window *window = Window::Create(s_settings.window);
        window->SetEventCallback(Engine::OnEvent);
        s_application->m_window = window;

        Rendering::RenderEngine::PreInitialize(s_settings.render, window);
    }

    //--------------------------------------------------------------
    void Engine::Initialize() {
        Audio::AudioEngine::Initialize();
        AssetManager::Initialize(s_settings.assets);
        Rendering::RenderEngine::Initialize();
        Physics::PhysicsEngine::Initialize();
        Scripting::ScriptingEngine::Initialize(s_settings.scripting);
        WorldManager::Initialize();
    }

    //--------------------------------------------------------------
    void Engine::PostInitialize() {
        Scripting::ScriptingEngine::PostInitialize();
    }

    //--------------------------------------------------------------
    uint32 Engine::Run() {
        s_running = true;

        s_application = Application::GetInstance();
        s_settings = ApplicationSettings();
        s_application->OnSetup(s_settings);

        PreInitialize();

        Initialize();
        s_application->OnInitialize();
        PostInitialize();
        s_application->GetWindow()->Show();

        s_time_stats.timer = Timer::Create();
        while (s_running) {
            HYP_PROFILE_FRAME("Main Thread");

            Iterate();
        }
        delete s_time_stats.timer;

        s_application->OnShutdown();
        Shutdown();

        return 0;
    }

    //--------------------------------------------------------------
    void Engine::Iterate() {
        const EngineLoopSystem &engine_loop = s_settings.core.engine_loop;
        ExecuteEngineLoopSubSystem(engine_loop.initilization);
        while (s_time_stats.accumulator > Time::GetFixedDeltaTime()) {
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
    void Engine::OnEvent(Event &event) {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<AppDisplayChangeEvent>([](AppDisplayChangeEvent &app_display_change_event) {
            Display::UpdateDisplayInfos();
        });

        dispatcher.Dispatch<WindowCloseEvent>([](WindowCloseEvent &window_close_event) {
            Exit();
        });
        dispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent &window_resize_event) {
            uint32 width = window_resize_event.GetWidth();
            uint32 height = window_resize_event.GetHeight();
            Display::UpdateSize(width, height);
        });

        dispatcher.Dispatch<KeyPressedEvent>([](KeyPressedEvent &key_pressed_event) {
            if (s_settings.core.allow_altf4) {
                if (key_pressed_event.HasKeyModifier(KeyModifier::Alt) && key_pressed_event.GetKeyCode() == KeyCode::F4) {
                    Exit();
                }
            }
        });

        Application::GetInstance()->OnEvent(event);
    }

    //--------------------------------------------------------------
    void Engine::Exit() {
        s_running = false;
    }

    //--------------------------------------------------------------
    void Engine::Shutdown() {
        // When shutting down we have to be very careful about the order.
        WorldManager::Shutdown();

        Scripting::ScriptingEngine::Shutdown();
        Physics::PhysicsEngine::Shutdown();
        Audio::AudioEngine::Shutdown();
        
        AssetManager::Shutdown();

        ObjectManager::Shutdown();
        Rendering::RenderEngine::Shutdown();

        delete Application::GetInstance()->GetWindow();
    }

    //--------------------------------------------------------------
    void Engine::ExecuteEngineLoopSubSystem(const EngineLoopSubSystem &engine_loop_sub_system) {
        // We explicitly ignore the update function for systems that contain sub systems.
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

    //--------------------------------------------------------------
    void Engine::TimeInitilization() {
        s_time_stats.frame++;
        float32 now = s_time_stats.timer->ElapsedSeconds();
        float32 delta_time = static_cast<float32>(now - s_time_stats.last_time);
        if (delta_time > Time::GetMaxDeltaTime()) {
            delta_time = Time::GetMaxDeltaTime();
        }
        Time::s_frame_time = delta_time * 1000.0f;
        Time::s_fps = static_cast<uint32>(1.0f / delta_time);
        s_time_stats.last_time = now;
        s_time_stats.accumulator += delta_time;
        Time::s_delta_time = delta_time;
        Time::s_time += delta_time;
        Time::s_time_since_engine_mode_change += delta_time;
    }

    //--------------------------------------------------------------
    void Engine::InputInitilization() {
        HYP_PROFILE_CATEGORY("Input", Optick::Category::Input);

        s_application->GetWindow()->Poll();
    }

    //--------------------------------------------------------------
    void Engine::PhysicsEngineFixedUpdate() {
        HYP_PROFILE_CATEGORY("Physics", Optick::Category::Physics);

        Physics::PhysicsEngine::FixedUpdate(Time::GetFixedDeltaTime());
    }

    //--------------------------------------------------------------
    void Engine::ScriptingEngineFixedUpdate() {
        Scripting::ScriptingEngine::FixedUpdate();
    }

    //--------------------------------------------------------------
    void Engine::ApplicationFixedUpdate() {
        s_application->OnFixedUpdate(Time::GetFixedDeltaTime());
    }

    //--------------------------------------------------------------
    void Engine::TimeFixedUpdate() {
        s_time_stats.accumulator -= Time::GetFixedDeltaTime();
    }

    //--------------------------------------------------------------
    void Engine::ApplicationTick() {
        s_application->OnTick();
    }

    //--------------------------------------------------------------
    void Engine::AssetManagerUpdate() {
        AssetManager::Update();
    }

    //--------------------------------------------------------------
    void Engine::WorldManagerUpdate() {
        WorldManager::Update(Time::GetDeltaTime());
    }

    //--------------------------------------------------------------
    void Engine::ScriptingEngineUpdate() {
        HYP_PROFILE_CATEGORY("ScriptingUpdate", Optick::Category::Script);

        Scripting::ScriptingEngine::Update();
    }

    //--------------------------------------------------------------
    void Engine::ApplicationUpdate() {
        HYP_PROFILE_CATEGORY("ApplicationUpdate", Optick::Category::GameLogic);

        s_application->OnUpdate(Time::GetDeltaTime());
    }

    //--------------------------------------------------------------
    void Engine::WorldManagerLateUpdate() {
        WorldManager::LateUpdate(Time::GetDeltaTime());
    }

    //--------------------------------------------------------------
    void Engine::ObjectManagerLateUpdate() {
        HYP_PROFILE_CATEGORY("LateUpdate", Optick::Category::GameLogic);

        ObjectManager::LateUpdate();
    }

    //--------------------------------------------------------------
    void Engine::RenderEngineLateUpdate() {
        HYP_PROFILE_CATEGORY("Rendering", Optick::Category::Rendering);

        Rendering::RenderEngine::Render();
    }

    //--------------------------------------------------------------
    void Engine::PanicInternal(const String &title, const String &message) {
        OperatingSystem::GetInstance()->DisplayError(title, message);
        Exit();
    }

}