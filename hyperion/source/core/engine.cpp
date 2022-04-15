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
    // Primitive types
    {
      MetaRegistry::Reflect<bool8>().Type("bool", MetaPrimitiveType::Bool);
      MetaRegistry::Reflect<int8>().Type("int8", MetaPrimitiveType::Int8);
      MetaRegistry::Reflect<int16>().Type("int16", MetaPrimitiveType::Int16);
      MetaRegistry::Reflect<int32>().Type("int32", MetaPrimitiveType::Int32);
      MetaRegistry::Reflect<int64>().Type("int64", MetaPrimitiveType::Int64);
      MetaRegistry::Reflect<uint8>().Type("uint8", MetaPrimitiveType::UInt8);
      MetaRegistry::Reflect<uint16>().Type("uint16", MetaPrimitiveType::UInt16);
      MetaRegistry::Reflect<uint32>().Type("uint32", MetaPrimitiveType::UInt32);
      MetaRegistry::Reflect<uint64>().Type("uint64", MetaPrimitiveType::UInt64);
      MetaRegistry::Reflect<float32>().Type("float32", MetaPrimitiveType::Float32);
      MetaRegistry::Reflect<float64>().Type("float64", MetaPrimitiveType::Float64);
      MetaRegistry::Reflect<Vector2>().Type("Vector2", MetaPrimitiveType::Vector2)
                                      .Property<&Vector2::x>("x")
                                      .Property<&Vector2::y>("y");
      MetaRegistry::Reflect<Vector3>().Type("Vector3", MetaPrimitiveType::Vector3)
                                      .Property<&Vector3::x>("x")
                                      .Property<&Vector3::y>("y")
                                      .Property<&Vector3::z>("z");
      MetaRegistry::Reflect<Vector4>().Type("Vector4", MetaPrimitiveType::Vector4)
                                      .Property<&Vector4::x>("x")
                                      .Property<&Vector4::y>("y")
                                      .Property<&Vector4::z>("z")
                                      .Property<&Vector4::w>("w");
      MetaRegistry::Reflect<Quaternion>().Type("Quaternion", MetaPrimitiveType::Quaternion)
                                         .Property<&Quaternion::x>("x")
                                         .Property<&Quaternion::y>("y")
                                         .Property<&Quaternion::z>("z")
                                         .Property<&Quaternion::w>("w");
      MetaRegistry::Reflect<Matrix4x4>().Type("Matrix4x4", MetaPrimitiveType::Matrix4x4)
                                        .Property<&Matrix4x4::m11>("m11")
                                        .Property<&Matrix4x4::m21>("m21")
                                        .Property<&Matrix4x4::m31>("m31")
                                        .Property<&Matrix4x4::m41>("m41")
                                        .Property<&Matrix4x4::m12>("m12")
                                        .Property<&Matrix4x4::m22>("m22")
                                        .Property<&Matrix4x4::m32>("m32")
                                        .Property<&Matrix4x4::m42>("m42")
                                        .Property<&Matrix4x4::m13>("m13")
                                        .Property<&Matrix4x4::m23>("m23")
                                        .Property<&Matrix4x4::m33>("m33")
                                        .Property<&Matrix4x4::m43>("m43")
                                        .Property<&Matrix4x4::m14>("m14")
                                        .Property<&Matrix4x4::m24>("m24")
                                        .Property<&Matrix4x4::m34>("m34")
                                        .Property<&Matrix4x4::m44>("m44");
      MetaRegistry::Reflect<String>().Type("String", MetaPrimitiveType::String);
      MetaRegistry::Reflect<Color>().Type("Color", MetaPrimitiveType::Color)
                                    .Property<&Color::r>("r")
                                    .Property<&Color::g>("g")
                                    .Property<&Color::b>("b")
                                    .Property<&Color::a>("a");
    }

    {
      MetaRegistry::Reflect<BoundingBox>("BoundingBox")
        .Property<&BoundingBox::min>("min")
        .Property<&BoundingBox::max>("max");
      MetaRegistry::Reflect<BoundingSphere>("BoundingSphere")
        .Property<&BoundingSphere::origin>("origin")
        .Property<&BoundingSphere::radius>("radius");
      MetaRegistry::Reflect<Plane>("Plane")
        .Property<&Plane::normal>("normal")
        .Property<&Plane::distance>("distance");
      MetaRegistry::Reflect<Ray>("Ray")
        .Property<&Ray::origin>("origin")
        .Property<&Ray::direction>("direction");
      MetaRegistry::Reflect<Rect>("Rect")
        .Property<&Rect::x>("x")
        .Property<&Rect::y>("y")
        .Property<&Rect::width>("width")
        .Property<&Rect::height>("height");
    }

    WorldManager::ReflectTypes();
    UI::UIFactory::ReflectTypes();
  }

  //--------------------------------------------------------------
  uint32 Engine::Run() {
    s_application = Application::GetInstance();
    s_application->RegisterTypes();
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
