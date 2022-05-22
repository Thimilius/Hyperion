//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/scripting/scripting_driver.hpp"
#include "hyperion/core/app/application_settings.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  class ScriptingEngine final {
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
