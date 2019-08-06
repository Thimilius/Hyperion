#pragma once

#include "hyperion/common.hpp"

#include "hyperion/math/vec2.hpp"

namespace Hyperion {

    enum class EWindowMode {
        Windowed,
        Borderless,
        Fullscreen
    };

    enum class EVSyncMode {
        DontSync,
        EveryVBlank,
        EverySecondVBlank
    };

    class CWindow {
    protected:
        CString m_title;
        Math::SVec2 m_size;
        EWindowMode m_window_mode;
        EVSyncMode m_vsync_mode;
    public:
        virtual ~CWindow() = default;

        inline CString GetTitle() const { return m_title; }
        virtual void SetTitle(CString title) = 0;
        inline Math::SVec2 GetSize() const { return m_size; }
        virtual void SetSize(Math::SVec2 size) = 0;
        inline EWindowMode GetWindowMode() const { return m_window_mode; }
        virtual void SetWindowMode(EWindowMode window_mode) = 0;
        inline EVSyncMode GetVSyncMode() const { return m_vsync_mode; }
        virtual void SetVSyncMode(EVSyncMode vsync_mode) = 0;

        virtual void Update() const = 0;

        static CWindow *Create(CString title, Math::SVec2 size, EWindowMode window_mode, EVSyncMode vsync_mode);
    };

}