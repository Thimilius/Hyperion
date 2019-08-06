#pragma once

#include "window.hpp"
#include "event.hpp"

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
        virtual void OnEvent(CEvent &event) = 0;
        virtual void OnFixedUpdate(float delta_time) = 0;
        virtual void OnUpdate(float delta_time) = 0;
        virtual void OnTick() = 0;
    private:
        void OnEventInternal(CEvent &event);
    };

    // This is a function to be defined by the client
    CApplication *CreateApplication();

}