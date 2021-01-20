#pragma once

#include "hyperion/scripting/scripting_driver.hpp"

namespace Hyperion {
    class Engine;
}

namespace Hyperion {

    class ScriptingEngine {
    private:
        ScriptingEngine() = delete;
        ~ScriptingEngine() = delete;

        static void Init();
        static void Shutdown();
    private:
        inline static IScriptingDriver *s_scripting_driver;
    private:
        friend class Hyperion::Engine;
    };

}