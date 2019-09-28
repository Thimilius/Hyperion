#pragma once

#include "hyperion/app/window.hpp"

namespace Hyperion {

    class CWindowsWindow : public CWindow {
    private:
        HWND m_window_handle;
        WINDOWPLACEMENT m_previous_placement;
    public:
        CWindowsWindow(const TString &title, u32 width, u32 height, EWindowMode window_mode, EVSyncMode vsync_mode);

        void SetTitle(const TString &title) override;
        void SetSize(u32 width, u32 height) override;
        void SetWindowMode(EWindowMode window_mode) override;
        void SetWindowState(EWindowState window_state) override;
        void SetVSyncMode(EVSyncMode vsync_mode) override;

        void SetIcon(const TString &path) override;
    private:
        void Update() const override;
        void Show() const override;

        EKeyCode TranslateKeyCode(u32 w_param, u32 l_param) const;
        EMouseButtonCode TranslateMouseButtonCode(u32 code) const;
        EKeyModifier GetKeyModifier() const;

        void CreateContext();
        void DispatchEvent(Events::CEvent &event) const;
        u32 GetMouseButtonFromMessage(u32 message, u32 w_param) const;

        static LRESULT CALLBACK MessageCallback(HWND window_handle, u32 message, WPARAM first_message_param, LPARAM second_message_param);
    };

}