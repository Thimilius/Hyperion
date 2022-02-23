//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/log.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/operating_system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  void Log::LogMessageInternal(LogLevel level, String message) {
    OperatingSystem::PrintToConsole(GetLogColor(level), message.c_str());
  }

  //--------------------------------------------------------------
  LogColor Log::GetLogColor(LogLevel level) {
    switch (level) {
      case LogLevel::Trace: return LogColor::White;
      case LogLevel::Info: return LogColor::DarkGreen;
      case LogLevel::Warning: return LogColor::DarkYellow;
      case LogLevel::Error: return LogColor::DarkRed;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return LogColor::White;
    }
  }

}