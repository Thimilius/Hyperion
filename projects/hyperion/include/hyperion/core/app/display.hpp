#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class DisplayInfo {
    public:
        struct DisplayModeInfo {
            u32 width;
            u32 height;
            u32 bits_per_pixel;
            u32 refresh_rate;

            inline bool operator==(const DisplayModeInfo &other) const {
                return width == other.width && height == other.height && bits_per_pixel == other.bits_per_pixel && refresh_rate == other.refresh_rate;
            }
        };
    private:
        String m_name;
        bool m_is_primary;
        Vector<DisplayModeInfo> m_mode_infos;
    public:
        inline const String &GetName() const { return m_name; }
        inline bool IsPrimary() const { return m_is_primary; }
        inline const Vector<DisplayModeInfo> &GetModeInfos() const { return m_mode_infos; }

        friend class Display;
    };

    class Display {
    private:
        inline static Vector<DisplayInfo> s_display_infos;
    public:
        inline static const Vector<DisplayInfo> &GetDisplayInfos() { return s_display_infos; }
        static DisplayInfo::DisplayModeInfo GetCurrentDisplayModeInfo();
    private:
        static void UpdateDisplayInfos();

        friend class Engine;
        friend class Application;
    };

}