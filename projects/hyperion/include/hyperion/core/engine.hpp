#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/operating_system.hpp"

namespace Hyperion {
    class Application;
    class Main;
    class Timer;
    class Event;
}

namespace Hyperion {

    class EngineLoop {
    protected:
        EngineLoop();
        ~EngineLoop();

        virtual void Iterate();
    private:
        Timer *m_timer;

        f64 m_last_time = 0.0;
        f64 m_accumulated_time = 0.0;
        f64 m_tick_timer = 0.0;
        u64 m_frame_counter = 0;
    private:
        friend class Hyperion::Engine;
    };

    class Engine final {
    public:
        template<typename ...Args>
        static void Panic(const String &system, const String &message_format, Args... args) {
            String title = StringUtils::Format("{} error!", system);
            OperatingSystem::GetInstance()->DisplayError(title, StringUtils::Format(message_format, args...));

            Exit();
        }
    private:
        Engine() = delete;
        ~Engine() = delete;

        inline static void SetSettings(const ApplicationSettings &settings) { s_settings = settings; }

        static void Setup();
        static void PreInit();
        static void Init();
        static u32 Run();
        static void Exit();
        static void Shutdown();

        static void OnEvent(Event &event);
        static void Update(f32 delta_time);
        static void LateUpdate();
        static void Render();
    private:
        inline static ApplicationSettings s_settings;
        inline static bool s_running = false;
        inline static EngineLoop *s_loop;
    private:
        friend class Hyperion::Application;
        friend class Hyperion::EngineLoop;
        friend class Hyperion::Main;
    };

}
