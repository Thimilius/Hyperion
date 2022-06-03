//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/scripting/scripting_engine.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/dotnet/dotnet_scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  //--------------------------------------------------------------
  void ScriptingEngine::Initialize(const ScriptingSettings &settings) {
    switch (settings.backend) {
      case ScriptingBackend::None: s_scripting_driver = new NullScriptingDriver(); break;
      case ScriptingBackend::Dotnet: s_scripting_driver = new DotnetScriptingDriver(); break;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }
    
    s_scripting_driver->Initialize();
  }

  //--------------------------------------------------------------
  void ScriptingEngine::PostInitialize() {
    s_scripting_driver->PostInitialize();
  }

  //--------------------------------------------------------------
  void ScriptingEngine::OnEngineModeChanged(EngineMode old_mode, EngineMode new_mode) {
    s_scripting_driver->OnEngineModeChanged(old_mode, new_mode);
  }

  //--------------------------------------------------------------
  void ScriptingEngine::Update() {
    s_scripting_driver->Update();
  }

  //--------------------------------------------------------------
  void ScriptingEngine::Shutdown() {
    s_scripting_driver->Shutdown();
    delete s_scripting_driver;
  }

}
