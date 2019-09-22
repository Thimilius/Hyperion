#include "hyppch.hpp"

#include "hyperion/system/log.hpp"

#include <time.h>

namespace Hyperion {

    CLog *CLog::s_instance = new CLog();
    
    const char *CLog::GetPrefixFormat(ELogType type) {
        switch (type) {
            case Hyperion::ELogType::Core: return "[%H:%M:%S] - [Core] - ";
            case Hyperion::ELogType::Client: return "[%H:%M:%S] - [Client] - ";
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return "";
        }
    }

    ELogColor CLog::GetLogColor(ELogLevel level) {
        switch (level) {
            case Hyperion::ELogLevel::Trace: return ELogColor::White;
            case Hyperion::ELogLevel::Info: return ELogColor::DarkGreen;
            case Hyperion::ELogLevel::Warning: return ELogColor::DarkYellow;
            case Hyperion::ELogLevel::Error: return ELogColor::DarkRed;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return ELogColor::White;
        }
    }

}