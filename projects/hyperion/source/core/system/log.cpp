#include "hyppch.hpp"

#include "hyperion/core/system/log.hpp"

#include "hyperion/core/system/operating_system.hpp"

namespace Hyperion {

    void Log::LogMessageInternal(LogLevel level, String message) {
        OperatingSystem::GetInstance()->PrintToConsole(GetLogColor(level), message.c_str());
    }
    
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