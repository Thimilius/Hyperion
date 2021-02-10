#include "hyppch.hpp"

#include "hyperion/scripting/scripting_engine.hpp"

#ifdef HYP_SCRIPTING_MONO
#include "hyperion/modules/mono/mono_scripting_driver.hpp"
#endif

namespace Hyperion::Scripting {

    uint64 ScriptingEngine::GetMemoryUsage() {
        return s_scripting_driver->GetMemoryUsage();
    }

    void ScriptingEngine::Initialize(const ScriptingSettings &settings) {
#ifdef HYP_SCRIPTING_MONO
        s_scripting_driver = new MonoScriptingDriver();
#else
        s_scripting_driver = new DummyScriptingDriver();
#endif

        s_scripting_driver->Initialize(settings);
    }

    void Scripting::ScriptingEngine::Update() {
        s_scripting_driver->Update();
    }

    void ScriptingEngine::Shutdown() {
        s_scripting_driver->Shutdown();
        delete s_scripting_driver;
    }

}