#pragma once

#include "hyperion/app/window.hpp"

namespace Hyperion {

    class CWindowsWindow : public CWindow {
    private:
        HWND m_window_handle;
        WINDOWPLACEMENT m_previous_placement;
    public:
        CWindowsWindow(const CString &title, u32 width, u32 height, EWindowMode window_mode);

        virtual void SetTitle(CString title) override;
        virtual void SetSize(u32 width, u32 height) override;
        virtual void SetWindowMode(EWindowMode window_mode) override;
        virtual void SetVSyncMode(EVSyncMode vsync_mode) override;
    private:
        virtual void Update() const override;

        void DispatchEvent(CEvent &event) const;

        static LRESULT CALLBACK MessageCallback(HWND window_handle, u32 message, WPARAM first_message_param, LPARAM second_message_param);
    };

}