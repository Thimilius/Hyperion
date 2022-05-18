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
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/app/events/app_event.hpp"
#include "hyperion/core/app/events/display_events.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/core/app/events/window_events.hpp"
#include "hyperion/core/memory/memory.hpp"
#include "hyperion/core/system/operating_system.hpp"
#include "hyperion/ecs/world/world_manager.hpp"
#include "hyperion/physics/physics_engine.hpp"
#include "hyperion/render/render_engine.hpp"
#include "hyperion/ui/ui_factory.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  EngineMode Engine::GetEngineMode() {
    if constexpr (IS_EDITOR) {
      return s_mode;
    } else {
      return EngineMode::Runtime;
    }
  }

  //--------------------------------------------------------------
  void Engine::Setup() {
    RegisterTypes();
    
    // We initialize the operating system first to get logging ability.
    OperatingSystem::Initialize();

    HYP_LOG_INFO("Engine", "Initializing...");
    OperatingSystemInfo operating_system_info = OperatingSystem::GetSystemInfo();
    HYP_LOG_INFO("Engine", "Processor Count: {} | Processor Frequency: {:.2f} Ghz | Physical Memory: {:.2f} GB",
                 operating_system_info.processor_info.processor_count,
                 static_cast<float32>(operating_system_info.processor_info.processor_mhz_frequency / 1000.0f),
                 static_cast<float32>(operating_system_info.memory_info.total_physical_memory / (1024.0f * 1024.0f * 1024.0f)));

    Display::UpdateDisplayInfos();
    DisplayInfo::DisplayModeInfo mode_info = Display::GetCurrentDisplayModeInfo();
    HYP_LOG_INFO("Engine", "Primary display: {}x{} @{} Hz", mode_info.width, mode_info.height, mode_info.refresh_rate);

    EngineLoopSystem &engine_loop = s_settings.core.engine_loop;
    engine_loop.initialization.name = "Initialization";
    engine_loop.initialization.sub_systems = {
      {
        "MemoryStatsInitialization", []() {
          HYP_PROFILE_SCOPE("EngineLoop.MemoryStatsInitialization");
          MemoryStats::ResetFrameMemory();
        }
      },
      {
        "TimeInitialization", []() {
          HYP_PROFILE_SCOPE("EngineLoop.TimeInitialization");

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
        }
      },
      {
        "InputInitialization", []() {
          HYP_PROFILE_SCOPE("EngineLoop.InputInitialization");
          s_application->GetMainWindow()->Poll();
        }
      },
      {
        "DisplayInitialization", []() {
          HYP_PROFILE_SCOPE("EngineLoop.DisplayInitialization");

          // NOTE: The order in which we check for this size change ist important.
          // We have to do it after the input initialization to properly handle resizing which might start inner engine iterations.

          Display::UpdateSize();
        }
      }
    };
    engine_loop.fixed_update.name = "FixedUpdate";
    engine_loop.fixed_update.sub_systems = {
      {
        "ApplicationFixedUpdate", []() {
          HYP_PROFILE_SCOPE("EngineLoop.ApplicationFixedUpdate");
          s_application->OnFixedUpdate(Time::GetFixedDeltaTime());
        }
      },
      {
        "TimeFixedUpdate", []() {
          HYP_PROFILE_SCOPE("EngineLoop.TimeFixedUpdate");
          Time::s_accumulator -= Time::GetFixedDeltaTime();
        }
      }
    };
    engine_loop.tick.name = "Tick";
    engine_loop.tick.sub_systems = {
      {
        "ApplicationTick", []() {
          HYP_PROFILE_SCOPE("EngineLoop.ApplicationTick");
          s_application->OnTick();
        }
      }
    };
    engine_loop.update.name = "Update";
    engine_loop.update.sub_systems = {
      {
        "AssetManagerPreUpdate", []() {
          HYP_PROFILE_SCOPE("EngineLoop.AssetManagerPreUpdate");
          AssetManager::PreUpdate();
        }
      },
      {
        "ApplicationUpdate", []() {
          HYP_PROFILE_SCOPE("EngineLoop.ApplicationUpdate");
          s_application->OnUpdate(Time::GetDeltaTime());
        }
      },
      {
        "WorldManagerUpdate", []() {
          HYP_PROFILE_SCOPE("EngineLoop.WorldManagerUpdate");
          WorldManager::Update();
        }
      }
    };
    engine_loop.late_update.name = "LateUpdate";
    engine_loop.late_update.sub_systems = {
      {
        "RenderEngineLateUpdate", []() {
          HYP_PROFILE_SCOPE("EngineLoop.RenderEngineLateUpdate");
          Rendering::RenderEngine::Render();
        }
      },
      {
        "AssetManagerLateUpdate", []() {
          HYP_PROFILE_SCOPE("EngineLoop.AssetManagerLateUpdate");
          AssetManager::LateUpdate();
        }
      },
      {
        "RenderEngineSynchronize", []() {
          HYP_PROFILE_SCOPE("EngineLoop.RenderEngineSynchronize");
          Rendering::RenderEngine::Present();
        }
      }
    };
  }

  //--------------------------------------------------------------
  void Engine::RegisterTypes() {
    Registration::class_<Guid>(HYP_NAME_OF_TYPE(Guid))
      .constructor();
    
    Registration::class_<Vector2>(HYP_NAME_OF_TYPE(Vector2))
      .constructor()
      .property("x", &Vector2::x)
      .property("y", &Vector2::y);

    Registration::class_<Vector3>(HYP_NAME_OF_TYPE(Vector3))
      .constructor()
      .property("x", &Vector3::x)
      .property("y", &Vector3::y)
      .property("z", &Vector3::z);

    Registration::class_<Vector4>(HYP_NAME_OF_TYPE(Vector4))
      .constructor()
      .property("x", &Vector4::x)
      .property("y", &Vector4::y)
      .property("z", &Vector4::z)
      .property("w", &Vector4::w);

    Registration::class_<Quaternion>(HYP_NAME_OF_TYPE(Quaternion))
      .constructor()
      .property("x", &Quaternion::x)
      .property("y", &Quaternion::y)
      .property("z", &Quaternion::z)
      .property("w", &Quaternion::w);

    Registration::class_<Matrix4x4>(HYP_NAME_OF_TYPE(Matrix4x4))
      .constructor()
      .property("m11", &Matrix4x4::m11)
      .property("m21", &Matrix4x4::m21)
      .property("m31", &Matrix4x4::m31)
      .property("m41", &Matrix4x4::m41)
      .property("m12", &Matrix4x4::m12)
      .property("m22", &Matrix4x4::m22)
      .property("m32", &Matrix4x4::m32)
      .property("m42", &Matrix4x4::m42)
      .property("m13", &Matrix4x4::m13)
      .property("m23", &Matrix4x4::m23)
      .property("m33", &Matrix4x4::m33)
      .property("m43", &Matrix4x4::m43)
      .property("m14", &Matrix4x4::m14)
      .property("m24", &Matrix4x4::m24)
      .property("m34", &Matrix4x4::m34)
      .property("m44", &Matrix4x4::m44);
    
    Registration::class_<Color>(HYP_NAME_OF_TYPE(Color))
      .constructor()
      .property("r", &Color::r)
      .property("g", &Color::g)
      .property("b", &Color::b)
      .property("a", &Color::a);

    Registration::class_<BoundingBox>(HYP_NAME_OF_TYPE(BoundingBox))
      .constructor()
      .property("min", &BoundingBox::min)
      .property("max", &BoundingBox::max);

    Registration::class_<BoundingSphere>(HYP_NAME_OF_TYPE(BoundingSphere))
      .constructor()
      .property("origin", &BoundingSphere::origin)
      .property("radius", &BoundingSphere::radius);

    Registration::class_<Plane>(HYP_NAME_OF_TYPE(Plane))
      .constructor()
      .property("normal", &Plane::normal)
      .property("distance", &Plane::distance);

    Registration::class_<Ray>(HYP_NAME_OF_TYPE(Ray))
      .constructor()
      .property("origin", &Ray::origin)
      .property("direction", &Ray::direction);

    Registration::class_<Rect>(HYP_NAME_OF_TYPE(Rect))
      .constructor()
      .property("x", &Rect::x)
      .property("y", &Rect::y)
      .property("width", &Rect::width)
      .property("height", &Rect::height);

    ComponentRegistry::RegisterTypes();
  }

  //--------------------------------------------------------------
  uint32 Engine::Run() {
    s_application = Application::GetInstance();
    s_application->OnRegisterTypes();
    s_application->OnSetup(s_settings);

    PreInitialize();

    Initialize();
    s_application->OnInitialize();
    PostInitialize();
    s_application->GetMainWindow()->Show();

    s_running = true;
    while (s_running) {
      HYP_PROFILE_FRAME("Main Thread");

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

#ifdef HYP_EDITOR
  //--------------------------------------------------------------
  void Engine::SetEngineMode(EngineMode mode) {
    s_mode = mode;
  }
#endif

  //--------------------------------------------------------------
  void Engine::PreInitialize() {
    HYP_ASSERT_MESSAGE(s_settings.core.max_delta_time > 0, "Max delta time must be greater than zero!");
    Time::s_max_delta_time = s_settings.core.max_delta_time;
    HYP_ASSERT_MESSAGE(s_settings.core.fixed_delta_time > 0, "Fixed delta time must be greater than zero!");
    Time::s_fixed_delta_time = s_settings.core.fixed_delta_time;

    Display::Initialize(s_settings.window.width, s_settings.window.height);

    Window *window = new Window(s_settings.window);
    window->SetAppEventCallback(Engine::OnAppEvent);
    s_application->m_main_window = window;

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
  void Engine::PostInitialize() { }

  //--------------------------------------------------------------
  void Engine::Iterate() {
    HYP_PROFILE_SCOPE("Engine.Iterate");

    const EngineLoopSystem &engine_loop = s_settings.core.engine_loop;

    {
      HYP_PROFILE_SCOPE("EngineLoop.Initialization");
      ExecuteEngineLoopSubSystem(engine_loop.initialization);
    }

    while (Time::s_accumulator > Time::GetFixedDeltaTime()) {
      HYP_PROFILE_SCOPE("EngineLoop.FixedUpdate");
      ExecuteEngineLoopSubSystem(engine_loop.fixed_update);
    }
    if (Time::OnInterval(1.0f)) {
      HYP_PROFILE_SCOPE("EngineLoop.Tick");
      ExecuteEngineLoopSubSystem(engine_loop.tick);
    }
    {
      HYP_PROFILE_SCOPE("EngineLoop.PreUpdate");
      ExecuteEngineLoopSubSystem(engine_loop.pre_update);
    }
    {
      HYP_PROFILE_SCOPE("EngineLoop.Update");
      ExecuteEngineLoopSubSystem(engine_loop.update);
    }
    {
      HYP_PROFILE_SCOPE("EngineLoop.LateUpdate");
      ExecuteEngineLoopSubSystem(engine_loop.late_update);
    }
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

    Rendering::RenderEngine::s_main_should_resize = false;
    dispatcher.Dispatch<WindowResizeAppEvent>([](WindowResizeAppEvent &window_resize_event) {
      Display::SetSize(window_resize_event.GetWidth(), window_resize_event.GetHeight());
      Rendering::RenderEngine::s_main_should_resize = true;
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

    delete s_application->GetMainWindow();
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
    OperatingSystem::DisplayError(title, message);
    Exit();
  }

}
