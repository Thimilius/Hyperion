//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/scripting/scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  class DotnetScriptingDriver final : public IScriptingDriver {
  public:
    void Initialize(const ScriptingSettings &settings) override;
    void Shutdown() override;
  };
    
}
