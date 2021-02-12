//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/system/log.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/operating_system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Log::LogMessageInternal(LogLevel level, String message) {
        OperatingSystem::GetInstance()->PrintToConsole(GetLogColor(level), message.c_str());
    }
    
    //--------------------------------------------------------------
    LogColor Log::GetLogColor(LogLevel level) {
        switch (level) {
            case Hyperion::LogLevel::Trace: return LogColor::White;
            case Hyperion::LogLevel::Info: return LogColor::DarkGreen;
            case Hyperion::LogLevel::Warning: return LogColor::DarkYellow;
            case Hyperion::LogLevel::Error: return LogColor::DarkRed;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return LogColor::White;
        }
    }

}