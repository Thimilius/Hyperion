#pragma once

#include "hyperion/system/operating_system.hpp"

namespace Hyperion {

    class CWindowsOperatingSystem : public COperatingSystem {
        EOperatingSystemType GetType() const override;
        SSystemInfo GetSystemInfo() const override;
    };

}