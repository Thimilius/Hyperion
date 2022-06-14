//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/system/engine_config.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  const char *EngineConfig::GetToolsPath() {
#ifdef HYP_PLATFORM_WINDOWS
    return "data/tools/windows";
#else
#error Platform not implemented
#endif
  }

  //--------------------------------------------------------------
  const char *EngineConfig::GetManagedCoreLibraryName() {
    return "Hyperion.Core.dll";
  }

  //--------------------------------------------------------------
  const char *EngineConfig::GetManagedLibrariesPath() {
    return "data/managed"; 
  }

}
