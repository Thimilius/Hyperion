#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Application;
    class Display;
    class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class DisplayInfo final {
    public:
        struct DisplayModeInfo {
            uint32 width;
            uint32 height;
            uint32 bits_per_pixel;
            uint32 refresh_rate;

            inline bool8 operator==(const DisplayModeInfo &other) const {
                return width == other.width && height == other.height && bits_per_pixel == other.bits_per_pixel && refresh_rate == other.refresh_rate;
            }
        };
    public:
        inline const String &GetName() const { return m_name; }
        inline bool8 IsPrimary() const { return m_is_primary; }
        inline const Array<DisplayModeInfo> &GetModeInfos() const { return m_mode_infos; }
    private:
        String m_name;
        bool8 m_is_primary;
        Array<DisplayModeInfo> m_mode_infos;
    private:
        friend class Hyperion::Display;
    };

    class Display final {
    public:
        inline static const Array<DisplayInfo> &GetDisplayInfos() { return s_display_infos; }
        static DisplayInfo::DisplayModeInfo GetCurrentDisplayModeInfo();

        inline static uint32 GetWidth() { return s_cached_width; }
        inline static uint32 GetHeight() { return s_cached_height; }
    private:
        Display() = delete;
        ~Display() = delete;
    private:
        static void UpdateDisplayInfos();

        inline static void UpdateSize(uint32 width, uint32 height) {
            s_cached_width = width;
            s_cached_height = height;
        }
    private:
        inline static Array<DisplayInfo> s_display_infos;

        inline static uint32 s_cached_width;
        inline static uint32 s_cached_height;
    private:
        friend class Hyperion::Application;
        friend class Hyperion::Engine;
    };

}