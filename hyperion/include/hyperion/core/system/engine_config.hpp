//------------------------ Header Guard ------------------------
#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class EngineConfig final {
  public:
    static const char *GetToolsPath();

    static const char *GetManagedCoreLibraryName();
    static const char *GetManagedLibrariesPath();
  private:
    EngineConfig() = delete;
    ~EngineConfig() = delete;
  };
  
}
