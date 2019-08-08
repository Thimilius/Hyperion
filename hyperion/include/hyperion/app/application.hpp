#pragma once

#include "hyperion/app/window.hpp"
#include "hyperion/app/events/events.hpp"

namespace Hyperion {

    class CApplication {
    private:
        static CApplication *s_instance;

        CWindow *m_window;
        bool m_running;
    public:
        CApplication(const CString& title, u32 width, u32 height, EWindowMode mode);
        virtual ~CApplication() = default;

        inline CWindow *GetWindow() const { return m_window; }

        int Run();
        void Exit();

        inline static CApplication *GetInstance() { return s_instance; }
    protected:
        virtual void OnInit() = 0;
        virtual void OnEvent(Events::CEvent &event) = 0;
        virtual void OnUpdate(float delta_time) = 0;
        virtual void OnRender() = 0;
        virtual void OnTick() = 0;
    private:
        void OnEventInternal(Events::CEvent &event);

        void OnKeyEvent(Events::CKeyEvent &event, bool down);
        void OnMouseButtonEvent(Events::CMouseButtonEvent &event, bool down);
    };

    // This is a function to be defined by the client
    CApplication *CreateApplication();

}