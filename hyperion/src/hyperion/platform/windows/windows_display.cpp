#include "hyppch.hpp"

#include "hyperion/app/display.hpp"

namespace Hyperion {

    void CDisplay::UpdateDisplayInfos() {
        DISPLAY_DEVICEA display_device = { 0 };
        display_device.cb = sizeof(display_device);

        s_display_infos.Clear();

        s32 device_number = 0;
        // Enumerate display devices
        while (EnumDisplayDevicesA(NULL, device_number, &display_device, 0)) {
            if (display_device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) {
                CDisplayInfo display_info;
                display_info.m_name = CString(display_device.DeviceName);

                DEVMODEA dev_mode = { 0 };
                dev_mode.dmSize = sizeof(dev_mode);
                u32 display_mode_count = 0;
                while (EnumDisplaySettingsA(display_device.DeviceName, display_mode_count, &dev_mode)) {
                    display_mode_count++;

                    // We only support a minium of 32 bits per pixel
                    if (dev_mode.dmPelsWidth < 32) {
                        continue;
                    }

                    CDisplayInfo::SDisplayModeInfo display_mode_info;
                    display_mode_info.width = dev_mode.dmPelsWidth;
                    display_mode_info.height = dev_mode.dmPelsHeight;
                    display_mode_info.refresh_rate = dev_mode.dmDisplayFrequency;
                    display_mode_info.bits_per_pixel = dev_mode.dmBitsPerPel;

                    if (display_info.m_mode_infos.Contains(display_mode_info)) {
                        continue;
                    }

                    display_info.m_mode_infos.Add(display_mode_info);
                }

                s_display_infos.Add(display_info);
            }

            device_number++;
        }
    }

}