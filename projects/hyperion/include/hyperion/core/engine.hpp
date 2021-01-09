#pragma once

#include "hyperion/core/operating_system.hpp"
#include "hyperion/core/threading/thread.hpp"
#include "hyperion/core/threading/auto_reset_event.hpp"
#include "hyperion/core/app/application_settings.hpp"

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
    
    class EngineSync final {
    private:
        inline static void NotifyRenderReady() { s_render_ready_event.Notify(); }
        inline static void WaitForRenderReady() { s_render_ready_event.Wait(); }

        inline static void NotifyUpdateReady() { s_update_ready_event.Notify(); }
        inline static void WaitForUpdateReady() { s_update_ready_event.Wait(); }

        inline static void NotifyRenderDone() { s_render_done_event.Notify(); }
        inline static void WaitForRenderDone() { s_render_done_event.Wait(); }

        inline static void NotifySwapDone() { s_swap_done_event.Notify(); }
        inline static void WaitForSwapDone() { s_swap_done_event.Wait(); }
    private:
        inline static Threading::AutoResetEvent s_render_ready_event = Threading::AutoResetEvent(false);
        inline static Threading::AutoResetEvent s_update_ready_event = Threading::AutoResetEvent(false);
        // We assume an already finished render loop for the very first frame.
        inline static Threading::AutoResetEvent s_render_done_event = Threading::AutoResetEvent(false);
        inline static Threading::AutoResetEvent s_swap_done_event = Threading::AutoResetEvent(false);
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Rendering::RenderEngine;
    };

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

        static void Setup();
        static void PreInit();
        static void Init();
        static u32 Run();
        static void Iterate();
        static void Exit();
        static void Shutdown();

        static void OnEvent(Event &event);
        static void Update(f32 delta_time);
        static void LateUpdate();
        static void Render();

        static void PanicInternal(const String &title, const String &message);
    private:
        inline static ApplicationSettings s_settings;
        inline static u64 s_update_frame = 0;
        inline static bool s_running = false;

        inline static struct LoopState {
            Timer *timer;
            f64 last_time = 0.0;
            f64 tick_timer = 0.0;
        } s_loop_state;
    private:
        friend class Hyperion::Application;
        friend class Hyperion::Main;
        friend class Hyperion::WindowsWindow;
    };

}
