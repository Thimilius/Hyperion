#include "hyppch.hpp"

#include "hyperion/system/log.hpp"

#include <time.h>

namespace Hyperion {

    CLog *CLog::s_instance = new CLog();

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