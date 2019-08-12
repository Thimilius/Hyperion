#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class CDisplayInfo {
    public:
        struct SDisplayModeInfo {
            u32 width;
            u32 height;
            u32 bits_per_pixel;
            u32 refresh_rate;

            inline bool operator==(const SDisplayModeInfo &other) const {
                return width == other.width && height == other.height && bits_per_pixel == other.bits_per_pixel && refresh_rate == other.refresh_rate;
            }
        };
    private:
        CString m_name;
        bool m_is_primary;
        CList<SDisplayModeInfo> m_mode_infos;
    public:
        inline const CString &GetName() const { return m_name; }
        inline bool IsPrimary() const { return m_is_primary; }
        inline const CList<SDisplayModeInfo> &GetModeInfos() const { return m_mode_infos; }

        friend class CDisplay;
    };

    class CDisplay {
    private:
        inline static CList<CDisplayInfo> s_display_infos;
    public:
        inline static const CList<CDisplayInfo> &GetDisplayInfos() { return s_display_infos; }
        static CDisplayInfo::SDisplayModeInfo GetCurrentDisplayModeInfo();
    private:
        static void UpdateDisplayInfos();

        friend class CEngine;
        friend class CApplication;
    };

}