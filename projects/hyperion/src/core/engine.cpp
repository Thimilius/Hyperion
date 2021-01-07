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
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/audio/audio_engine.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/font.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/physics/physics_engine.hpp"
#include "hyperion/scripting/scripting_engine.hpp"

namespace Hyperion {
    
    EngineLoop::EngineLoop() {
        m_timer = Timer::Create();
    }

    EngineLoop::~EngineLoop() {
        delete m_timer;
    }

    void EngineLoop::Iterate() {
        Application *application = Application::GetInstance();

        f32 now = m_timer->ElapsedSeconds();
        f32 delta_time = static_cast<f32>(now - m_last_time);
        if (delta_time > Time::GetMaxDeltaTime()) {
            delta_time = Time::GetMaxDeltaTime();
        }
        m_last_time = now;
        m_tick_timer += delta_time;
        Time::s_delta_time = delta_time;
        Time::s_time += delta_time;

        m_frame_counter++;

        Engine::Update(delta_time);
        application->OnUpdate(delta_time);
        Engine::LateUpdate();

        Engine::Render();

        if (m_tick_timer > 1.0f) {
            u32 fps = static_cast<u32>(m_frame_counter * (1.0 / m_tick_timer));
            Time::s_fps = fps;
            Time::s_frame_time = 1000.0 / fps;

            application->OnTick();

            m_frame_counter = 0;
            m_tick_timer = 0;
        }

        application->GetWindow()->Update();
    }

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
        s_loop = new EngineLoop();

        HYP_ASSERT_MESSAGE(s_settings.core.max_delta_time > 0, "Max delta time must be greater than zero!");
        Time::s_max_delta_time = s_settings.core.max_delta_time;

        Display::UpdateSize(s_settings.window.width, s_settings.window.height);

        Window *window = Window::Create(s_settings.window, s_settings.render.backend);
        window->SetEventCallback(Engine::OnEvent);
        Application::GetInstance()->m_window = window;

        Rendering::RenderEngine::PreInit(s_settings.render);
    }

    void Engine::Init() {
        AssetManager::Init(s_settings.assets);
        Rendering::RenderEngine::Init(s_settings.render);
        Rendering::Font::Init();
        Physics::PhysicsEngine::Init();
        Audio::AudioEngine::Init();
        ScriptingEngine::Init();
    }

    u32 Engine::Run() {
        Application *application = Application::GetInstance();
        HYP_ASSERT_NO_MESSAGE(application);

        s_running = true;

        PreInit();
        Init();
        application->OnInit();

        application->GetWindow()->Show();

        while (s_running) {
            s_loop->Iterate();
        }

        application->OnShutdown();
        Shutdown();

        return 0;
    }

    void Engine::Exit() {
        s_running = false;
    }

    void Engine::Shutdown() {
        WorldManager::Shutdown();
        ScriptingEngine::Shutdown();
        Audio::AudioEngine::Shutdown();
        Physics::PhysicsEngine::Shutdown();
        Rendering::RenderEngine::Shutdown();
        AssetManager::Shutdown();

        delete Application::GetInstance()->GetWindow();
        delete s_loop;
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
            // Explicitly handle alt-f4 for closing
            if (key_pressed_event.HasKeyModifier(KeyModifier::Alt) && key_pressed_event.GetKeyCode() == KeyCode::F4) {
                Exit();
            }
        });

        // Forward event to client
        Application::GetInstance()->OnEvent(event);
    }

    void Engine::Update(f32 delta_time) {
        AssetManager::Update();
        Physics::PhysicsEngine::Update(delta_time);
        WorldManager::Update(delta_time);
    }

    void Engine::LateUpdate() {
        ObjectManager::LateUpdate();
    }

    void Engine::Render() {
        Rendering::RenderEngine::Render();
    }

}