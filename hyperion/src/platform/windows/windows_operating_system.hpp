#pragma once

#include "hyperion/system/operating_system.hpp"

namespace Hyperion {

    class WindowsOperatingSystem : public OperatingSystem {
        OperatingSystemType GetType() const override;
        SystemInfo GetSystemInfo() const override;
    };

}