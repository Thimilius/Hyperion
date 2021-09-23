#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/operating_system.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class AppEvent;
    class Application;
    class Main;
    class WindowsWindow;

    namespace Rendering {
        class RenderEngine;
    }
}

//-------------------- Definition Namespace --------------------
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
        static void RegisterTypes();
        static uint32 Run();
        static void Exit();

        static void PreInitialize();
        static void Initialize();
        static void PostInitialize();
        static void Iterate();
        static void OnAppEvent(AppEvent &app_event);
        static void Shutdown();
        
        static void ExecuteEngineLoopSubSystem(const EngineLoopSubSystem &engine_loop_sub_system);

        static void PanicInternal(const String &title, const String &message);
    private:
        inline static ApplicationSettings s_settings;
        inline static Application *s_application;
        inline static std::atomic<bool8> s_running = false;
    private:
        friend class Hyperion::Application;
        friend class Hyperion::Main;
        friend class Hyperion::WindowsWindow;
    };

}

#define HYP_PANIC do {                                                     \
        HYP_LOG_ERROR("Engine", "Engine encountered an error!");           \
        HYP_DEBUG_BREAK;                                                   \
        Hyperion::Engine::Panic("Engine", "Engine encountered an error!"); \
    } while(false);
#define HYP_PANIC_MESSAGE(s, m, ...) do {                 \
        HYP_LOG_ERROR((s), (m), ##__VA_ARGS__);           \
        HYP_DEBUG_BREAK;                                  \
        Hyperion::Engine::Panic((s), (m), ##__VA_ARGS__); \
    } while(false);
