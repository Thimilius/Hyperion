//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/app/display.hpp"

//---------------------- Library Includes ----------------------
#include <Windows.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    DisplayInfo::DisplayModeInfo Display::GetCurrentDisplayModeInfo() {
        DISPLAY_DEVICEA display_device = { 0 };
        display_device.cb = sizeof(display_device);

        int32 device_number = 0;
        while (EnumDisplayDevicesA(nullptr, device_number, &display_device, 0)) {
            device_number++;

            if (display_device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP && display_device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
                DEVMODEA dev_mode = { 0 };
                dev_mode.dmSize = sizeof(dev_mode);
                uint32 display_mode_number = 0;

                if (EnumDisplaySettingsA(display_device.DeviceName, ENUM_CURRENT_SETTINGS, &dev_mode)) {
                    DisplayInfo::DisplayModeInfo result;
                    result.width = dev_mode.dmPelsWidth;
                    result.height = dev_mode.dmPelsHeight;
                    result.refresh_rate = dev_mode.dmDisplayFrequency;
                    result.bits_per_pixel = dev_mode.dmBitsPerPel;
                        
                    return result;
                }
            }
        }

        HYP_ASSERT_MESSAGE(false, "Windows does not have a primary display device set!");

        return DisplayInfo::DisplayModeInfo();
    }

    //--------------------------------------------------------------
    void Display::Initialize(uint32 width, uint32 height) {
        s_width = width;
        s_height = height;
        s_last_width = width;
        s_last_height = height;
        s_size_changed = true;
    }

    //--------------------------------------------------------------
    void Display::UpdateSize() {
        s_size_changed = false;
        if (s_width != s_last_width || s_height != s_last_height) {
            s_last_width = s_width;
            s_last_height = s_height;
            s_size_changed = true;
        }
    }

    //--------------------------------------------------------------
    void Display::SetSize(uint32 width, uint32 height) {
        s_last_width = Display::s_width;
        s_last_height = Display::s_height;
        s_width = width;
        s_height = height;
    }

    //--------------------------------------------------------------
    void Display::UpdateDisplayInfos() {
        DISPLAY_DEVICEA display_device = { 0 };
        display_device.cb = sizeof(display_device);

        s_display_infos.Clear();

        int32 device_number = 0;
        while (EnumDisplayDevicesA(nullptr, device_number, &display_device, 0)) {
            device_number++;

            if (display_device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) {
                DisplayInfo display_info;
                display_info.m_name = String(display_device.DeviceName);
                display_info.m_is_primary = display_device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE;

                DEVMODEA dev_mode = { 0 };
                dev_mode.dmSize = sizeof(dev_mode);
                uint32 display_mode_number = 0;
                while (EnumDisplaySettingsA(display_device.DeviceName, display_mode_number, &dev_mode)) {
                    display_mode_number++;

                    // We only support a modes with a minimum of 24 bits per pixel.
                    if (dev_mode.dmPelsWidth < 24) {
                        continue;
                    }

                    DisplayInfo::DisplayModeInfo display_mode_info;
                    display_mode_info.width = dev_mode.dmPelsWidth;
                    display_mode_info.height = dev_mode.dmPelsHeight;
                    display_mode_info.refresh_rate = dev_mode.dmDisplayFrequency;
                    display_mode_info.bits_per_pixel = dev_mode.dmBitsPerPel;

                    // Skip duplicate modes.
                    if (std::find(display_info.m_mode_infos.begin(), display_info.m_mode_infos.end(), display_mode_info) != display_info.m_mode_infos.end()) {
                        continue;
                    }

                    display_info.m_mode_infos.Add(display_mode_info);
                }

                s_display_infos.Add(display_info);
            }
        }
    }

}