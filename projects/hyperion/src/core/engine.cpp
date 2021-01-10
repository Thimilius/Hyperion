#include "hyppch.hpp"

#include "hyperion/core/engine.hpp"

#include "hyperion/core/timer.hpp"
#include "hyperion/core/object_manager.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/app/events/app_events.hpp"
#include "hyperion/core/app/events/window_events.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/audio/audio_engine.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/physics/physics_engine.hpp"
#include "hyperion/scripting/scripting_engine.hpp"

namespace Hyperion {

    void Engine::Setup() {
        // We initialize the operating system first to get logging ability
        OperatingSystem::GetInstance()->Init();

        HYP_LOG_INFO("Engine", "Initializing...");
        SystemInfo system_info = OperatingSystem::GetInstance()->GetSystemInfo();
        HYP_LOG_INFO("Engine", "Processor Count: {} | Processor Frequency: {:.2f} Ghz | Physical Memory: {:.2f} GB",
            system_info.processor_info.processor_count,
            static_cast<f32>(system_info.processor_info.processor_mhz_frequency / 1000.0f),
            static_cast<f32>(system_info.memory_info.total_physical_memory / (1024.0f * 1024.0f * 1024.0f)));

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

        Window *window = Window::Create(s_settings.window);
        window->SetEventCallback(Engine::OnEvent);
        Application::GetInstance()->m_window = window;

        Rendering::RenderEngine::PreInit(s_settings.render, window);
    }

    void Engine::Init() {
        Rendering::RenderEngine::Init();
        Physics::PhysicsEngine::Init();
        Audio::AudioEngine::Init();
        ScriptingEngine::Init();
    }

    u32 Engine::Run() {
        Application *application = Application::GetInstance();
        HYP_ASSERT(application);

        s_running = true;

        PreInit();
        Init();

        Window *window = application->GetWindow();
        application->OnInit();
        window->Show();

        if (s_settings.render.threading_mode == Rendering::RenderThreadingMode::MultiThreaded) {
            EngineSync::NotifyUpdateReady();
            EngineSync::WaitForRenderReady();
        }

        s_stats.timer = Timer::Create();
        while (s_running) {
            Iterate();
        }
        delete s_stats.timer;

        application->OnShutdown();
        Shutdown();

        return 0;
    }

    void Engine::Iterate() {
        Application *application = Application::GetInstance();
        Window *window = application->GetWindow();

        s_stats.frame++;

        window->Update();

        f32 now = s_stats.timer->ElapsedSeconds();
        f32 delta_time = static_cast<f32>(now - s_stats.last_time);
        if (delta_time > Time::GetMaxDeltaTime()) {
            delta_time = Time::GetMaxDeltaTime();
        }
        s_stats.last_time = now;
        s_stats.tick_timer += delta_time;
        s_stats.accumulator += delta_time;
        Time::s_delta_time = delta_time;
        Time::s_time += delta_time;

        Update(delta_time);
        application->OnUpdate(delta_time);
        LateUpdate();

        f32 fixed_delta_time = Time::GetFixedDeltaTime();
        while (s_stats.accumulator > fixed_delta_time) {
            FixedUpdate(fixed_delta_time);
            application->OnFixedUpdate(fixed_delta_time);

            s_stats.accumulator -= fixed_delta_time;
        }

        if (s_stats.tick_timer > 1.0f) {
            u32 fps = static_cast<u32>(1.0 / delta_time);
            Time::s_fps = fps;
            Time::s_frame_time = 1000.0 / fps;

            application->OnTick();

            s_stats.tick_timer = 0.0;
        }

        Render();
    }

    void Engine::Exit() {
        s_running = false;
        Rendering::RenderEngine::Exit();
    }

    void Engine::Shutdown() {
        WorldManager::Shutdown();
        ScriptingEngine::Shutdown();
        Audio::AudioEngine::Shutdown();
        Physics::PhysicsEngine::Shutdown();
        Rendering::RenderEngine::Shutdown();

        delete Application::GetInstance()->GetWindow();
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
            u32 width = window_resize_event.GetWidth();
            u32 height = window_resize_event.GetHeight();
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

    void Engine::Update(f32 delta_time) {
        WorldManager::Update(delta_time);
    }

    void Engine::FixedUpdate(f32 delta_time) {
        Physics::PhysicsEngine::FixedUpdate(delta_time);
    }

    void Engine::LateUpdate() {
        ObjectManager::LateUpdate();
    }

    void Engine::Render() {
        Rendering::RenderEngine::Render();
    }

    void Engine::PanicInternal(const String &title, const String &message) {
        OperatingSystem::GetInstance()->DisplayError(title, message);
        Exit();
    }

}