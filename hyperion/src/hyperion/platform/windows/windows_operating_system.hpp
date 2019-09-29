#pragma once

#include "hyperion/system/operating_system.hpp"

#include <Windows.h>

namespace Hyperion {

    class CWindowsOperatingSystem : public COperatingSystem {
    private:
        HANDLE m_console_handle;
    public:
        void Init() override;

        EOperatingSystemType GetType() const override;
        SSystemInfo GetSystemInfo() const override;

        void PrintToConsole(ELogColor color, const TString &message) const override;
    };

}