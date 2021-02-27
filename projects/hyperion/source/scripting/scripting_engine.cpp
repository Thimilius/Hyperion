//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/scripting/scripting_engine.hpp"

//---------------------- Project Includes ----------------------
#ifdef HYP_SCRIPTING_MONO
#include "hyperion/modules/mono/mono_scripting_driver.hpp"
#endif

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    uint64 ScriptingEngine::GetMemoryUsage() {
        return s_scripting_driver->GetMemoryUsage();
    }

    //--------------------------------------------------------------
    void ScriptingEngine::Initialize(const ScriptingSettings &settings) {
//#ifdef HYP_SCRIPTING_MONO
//        s_scripting_driver = new MonoScriptingDriver();
//#else
//        s_scripting_driver = new DummyScriptingDriver();
//#endif
        // TEMP: This is just so we currently do not need to bother about scripting at all and can focus on other things.
        s_scripting_driver = new DummyScriptingDriver();

        s_scripting_driver->Initialize(settings);
    }

    //--------------------------------------------------------------
    void ScriptingEngine::EngineModeChanged(EngineMode engine_mode) {
        s_scripting_driver->EngineModeChange(engine_mode);
    }

    //--------------------------------------------------------------
    void Scripting::ScriptingEngine::Update() {
        s_scripting_driver->Update();
    }

    //--------------------------------------------------------------
    void ScriptingEngine::Shutdown() {
        s_scripting_driver->Shutdown();
        delete s_scripting_driver;
    }

}