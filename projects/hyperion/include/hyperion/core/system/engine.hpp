#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/system/operating_system.hpp"

namespace Hyperion {
    class Application;
    class Event;
    class Main;
    class Timer;
    class WindowsWindow;

    namespace Rendering {
        class RenderEngine;
    }
}

namespace Hyperion {
    
    class Engine final {
    public:
        template<typename ...Args>
        static void Panic(const String &system, const String &message_format, Args... args) {
            String title = StringUtils::Format("{} error!", system);
            String message = StringUtils::Format(message_format, args...);
            PanicInternal(title, message);
        }
    private:
        Engine() = delete;
        ~Engine() = delete;
    private:
        static void Setup();
        static void PreInitialize();
        static void Initialize();
        static uint32 Run();
        static void Iterate();
        static void OnEvent(Event &event);
        static void Exit();
        static void Shutdown();

        static void ExecuteEngineLoopSubSystem(const EngineLoopSubSystem &engine_loop_sub_system);

        static void TimeInitilization();
        static void InputInitilization();
        static void PhysicsEngineFixedUpdate();
        static void ApplicationFixedUpdate();
        static void TimeFixedUpdate();
        static void TimeTick();
        static void ApplicationTick();
        static void AssetManagerUpdate();
        static void WorldManagerUpdate();
        static void ApplicationUpdate();
        static void ObjectManagerLateUpdate();
        static void RenderEngineLateUpdate();

        static void PanicInternal(const String &title, const String &message);
    private:
        inline static ApplicationSettings s_settings;
        inline static Application *s_application;
        inline static bool s_running = false;

        inline static struct Stats {
            Timer *timer;

            float64 last_time = 0.0;
            float64 accumulator = 0.0;

            uint64 frame = 0;
            uint32 fps_counter = 0;
        } s_stats;
    private:
        friend class Hyperion::Application;
        friend struct Hyperion::EngineLoopSystem;
        friend class Hyperion::Main;
        friend class Hyperion::WindowsWindow;
    };

}
