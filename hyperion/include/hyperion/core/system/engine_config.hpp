//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class EngineConfig final {
  public:
    static String GetToolsPath();

    static String GetManagedLibrariesPath();
  private:
    EngineConfig() = delete;
    ~EngineConfig() = delete;
  };
  
}
