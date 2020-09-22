#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {
    class Application;
    class Display;
    class Engine;
}

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
    public:
        inline const String &GetName() const { return m_name; }
        inline bool IsPrimary() const { return m_is_primary; }
        inline const Vector<DisplayModeInfo> &GetModeInfos() const { return m_mode_infos; }
    private:
        String m_name;
        bool m_is_primary;
        Vector<DisplayModeInfo> m_mode_infos;
    private:
        friend class Hyperion::Display;
    };

    class Display {
    public:
        inline static const Vector<DisplayInfo> &GetDisplayInfos() { return s_display_infos; }
        static DisplayInfo::DisplayModeInfo GetCurrentDisplayModeInfo();

        inline static u32 GetWidth() { return s_cached_width; }
        inline static u32 GetHeight() { return s_cached_height; }
    private:
        Display() = delete;
        ~Display() = delete;

        static void UpdateDisplayInfos();

        inline static void UpdateSize(u32 width, u32 height) {
            s_cached_width = width;
            s_cached_height = height;
        }
    private:
        inline static Vector<DisplayInfo> s_display_infos;

        inline static u32 s_cached_width;
        inline static u32 s_cached_height;
    private:
        friend class Hyperion::Application;
        friend class Hyperion::Engine;
    };

}