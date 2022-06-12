//------------------------ Header Guard ------------------------
#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  class IScriptingWorld {
  public:
    virtual ~IScriptingWorld() = default;
  };

  class NullScriptingWorld final : public IScriptingWorld {
     
  };
  
}
