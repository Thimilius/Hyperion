//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/log.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  class IScriptingDriver {
  public:
    virtual ~IScriptingDriver() = default;
  public:
    virtual void Initialize() = 0;
    virtual void PostInitialize() = 0;
    virtual void Update() = 0;
    virtual void Shutdown() = 0;
  };

  class NullScriptingDriver final : public IScriptingDriver {
  public:
    void Initialize() override { HYP_LOG_INFO("Scripting", "Initialized Null scripting driver!"); }
    void PostInitialize() override { }
    void Update() override { }
    void Shutdown() override { }
  };
    
}
