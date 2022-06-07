//------------------------ Header Guard ------------------------
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

    inline static bool8 HasChangedSize() { return s_size_changed; }

    inline static uint32 GetWidth() { return s_width; }
    inline static uint32 GetHeight() { return s_height; }

#ifdef HYP_EDITOR
    inline static uint32 GetPreviewWidth() { return s_preview_width; }
    inline static uint32 GetPreviewHeight() { return s_preview_height; }
    static void SetPreviewSize(uint32 width, uint32 height);
#endif
  private:
    Display() = delete;
    ~Display() = delete;
  private:
    static void Initialize(uint32 width, uint32 height);

    static void UpdateSize();
    static void SetSize(uint32 width, uint32 height);

    static void UpdateDisplayInfos();
  private:
    inline static Array<DisplayInfo> s_display_infos;

    inline static uint32 s_width;
    inline static uint32 s_height;
    inline static uint32 s_last_width;
    inline static uint32 s_last_height;
    inline static bool8 s_size_changed = false;

#ifdef HYP_EDITOR
    inline static uint32 s_preview_width;
    inline static uint32 s_preview_height;
#endif
  private:
    friend class Hyperion::Application;
    friend class Hyperion::Engine;
  };

}
