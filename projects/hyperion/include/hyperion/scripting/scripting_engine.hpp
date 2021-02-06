#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/scripting/scripting_driver.hpp"

namespace Hyperion {
    class Engine;
}

namespace Hyperion {

    class ScriptingEngine {
    private:
        ScriptingEngine() = delete;
        ~ScriptingEngine() = delete;
    private:
        static void Initialize(const ScriptingSettings &settings);
        static void Shutdown();
    private:
        inline static IScriptingDriver *s_scripting_driver;
    private:
        friend class Hyperion::Engine;
    };

}