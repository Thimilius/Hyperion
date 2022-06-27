//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/log.hpp"

//---------------- C Standard Library Includes -----------------
#include <ctime>

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/operating_system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  void ConsoleLogger::Log(LogLevel level, const String &message) {
    OperatingSystem::PrintToConsole(GetLogColor(level), message);
  }
  
  //--------------------------------------------------------------
  ConsoleColor ConsoleLogger::GetLogColor(LogLevel level) {
    switch (level) {
      case LogLevel::Trace: return ConsoleColor::White;
      case LogLevel::Info: return ConsoleColor::DarkGreen;
      case LogLevel::Warning: return ConsoleColor::DarkYellow;
      case LogLevel::Error: return ConsoleColor::DarkRed;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return ConsoleColor::White;
    }
  }

  //--------------------------------------------------------------
  void Log::LogMessageInternal(LogType type, LogLevel level, const char *system, const String &formatted_message) {
    std::time_t current_time = std::time(&current_time);
    std::tm *time_info = std::localtime(&current_time);
    char prefix_buffer[30];
    strftime(prefix_buffer, sizeof(prefix_buffer), "[%H:%M:%S]", time_info);
    
    String final_message = StringUtils::Format("{} - [{}] - {}\n", prefix_buffer, system, formatted_message);

    for (ILogger *logger : s_loggers) {
      logger->Log(level, final_message);
    }
  }

}
