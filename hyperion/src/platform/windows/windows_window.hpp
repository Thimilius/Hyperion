#pragma once

#include "hyperion/app/window.hpp"

namespace Hyperion {

    class CWindowsWindow : public CWindow {
    private:
        HWND m_window_handle;
        HDC m_device_context;
        WINDOWPLACEMENT m_previous_placement;
    public:
        CWindowsWindow(CString title, Math::SVec2 size, EWindowMode window_mode, EVSyncMode vsync_mode);

        virtual void SetTitle(CString title) override;
        virtual void SetSize(Math::SVec2 size) override;
        virtual void SetWindowMode(EWindowMode window_mode) override;
        virtual void SetVSyncMode(EVSyncMode vsync_mode) override;

        virtual void Update() const override;
    };

}