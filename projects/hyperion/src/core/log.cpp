#include "hyppch.hpp"

#include "hyperion/core/log.hpp"

#include <ctime>

namespace Hyperion {

    Log *Log::s_instance = new Log();

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