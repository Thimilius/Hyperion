#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/operating_system.hpp"
#include "hyperion/rendering/forward_render_pipeline.hpp"

namespace Hyperion {

    class Engine {
    private:
        inline static Scope<Rendering::RenderPipeline> s_render_pipeline;
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
        static void Init(const ApplicationSettings &settings);
        static void Update(f32 delta_time);
        static void LateUpdate();
        static void Render();
        static void Tick();
        static void Shutdown();

        static Rendering::RenderPipeline *GetRenderPipeline(const RenderSettings &settings);

        friend class Main;
        friend class Application;
    };

}
