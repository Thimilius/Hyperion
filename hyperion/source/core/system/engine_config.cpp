//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/system/engine_config.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  String EngineConfig::GetToolsPath() {
#ifdef HYP_PLATFORM_WINDOWS
    return "data/tools/windows";
#else
#error Platform not implemented
#endif
  }

  //--------------------------------------------------------------
  String EngineConfig::GetManagedLibrariesPath() {
    return "data/managed"; 
  }

}
