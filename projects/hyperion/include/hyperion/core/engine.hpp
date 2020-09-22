#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/operating_system.hpp"

namespace Hyperion {
    class Application;
    class Main;
}

namespace Hyperion {

    class Engine {
    public:
        template<typename ...Args>
        static void Panic(const String &system, const String &message_format, Args... args) {
            String title = StringUtils::Format("{} error!", system);
            OperatingSystem::GetInstance()->DisplayError(title, StringUtils::Format(message_format, args...));

            // This is not really a good way of exiting as it does not necessarily free all relevant resources
            exit(-1);
        }
    private:
        Engine() = delete;
        ~Engine() = delete;

        static void Setup();
        static void PreInit(const ApplicationSettings &settings);
        static void Init(const ApplicationSettings &settings);
        static void Update(f32 delta_time);
        static void LateUpdate();
        static void Render();
        static void Shutdown();
    private:
        friend class Hyperion::Application;
        friend class Hyperion::Main;
    };

}
