#pragma once

#include "hyperion/core/app/application.hpp"

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

        static void Init(const ApplicationSettings &settings);
        static void Setup(const ApplicationSettings &settings);
        static void Update();
        static void PostUpdate();
        static void Shutdown();

        friend class Application;
    };

}