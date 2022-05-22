﻿//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  class IScriptingDriver {
  public:
    virtual ~IScriptingDriver() = default;
  public:
    virtual void Initialize(const ScriptingSettings &settings) = 0;
    virtual void Update() = 0;
    virtual void Shutdown() = 0;
  };

  class NullScriptingDriver final : public IScriptingDriver {
  public:
    void Initialize(const ScriptingSettings &settings) override { HYP_LOG_INFO("Scripting", "Initialized Null scripting driver!"); }
    void Update() override { }
    void Shutdown() override { }
  };
    
}
