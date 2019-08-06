#include "hyppch.hpp"

#include "hyperion/util/log.hpp"

namespace Hyperion {

    const char *CLog::GetPrefixFormat(ELogType type) {
        switch (type) {
            case Hyperion::ELogType::Core: return "[%H:%M:%S] - [Core] - ";
            case Hyperion::ELogType::OpenGL: return "[%H:%M:%S] - [OpenGL] - ";
            case Hyperion::ELogType::Client: return "[%H:%M:%S] - [Client] - ";
            default: HYP_ASSERT(false); return "";
        }
    }

    ELogColor CLog::GetLogColor(ELogLevel level) {
        switch (level) {
            case Hyperion::ELogLevel::Trace: return ELogColor::White;
            case Hyperion::ELogLevel::Info: return ELogColor::Green;
            case Hyperion::ELogLevel::Warning: return ELogColor::Yellow;
            case Hyperion::ELogLevel::Error: return ELogColor::Red;
            default: HYP_ASSERT(false); return ELogColor::White;
        }
    }

}