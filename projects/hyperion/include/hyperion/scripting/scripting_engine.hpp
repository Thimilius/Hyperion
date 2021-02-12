#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/scripting/scripting_driver.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class ScriptingEngine {
    public:
        static uint64 GetMemoryUsage();
    private:
        ScriptingEngine() = delete;
        ~ScriptingEngine() = delete;
    private:
        static void Initialize(const ScriptingSettings &settings);
        static void Update();
        static void Shutdown();
    private:
        inline static IScriptingDriver *s_scripting_driver;
    private:
        friend class Hyperion::Engine;
    };

}