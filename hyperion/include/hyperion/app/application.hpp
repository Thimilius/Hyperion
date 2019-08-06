#pragma once

#include "window.hpp"

namespace Hyperion {

    class CApplication {
    private:
        static CApplication *s_instance;

        CWindow *m_window;
        bool m_running;
    public:
        CApplication();
        virtual ~CApplication() = default;

        inline CWindow *GetWindow() const { return m_window; }

        int Run();
        void Exit();

        virtual void OnInit() = 0;
        virtual void OnUpdate(float delta_time) = 0;
        virtual void OnTick() = 0;

        inline static CApplication *GetInstance() { return s_instance; }
    };

    // This is a function to be defined by the client
    CApplication *CreateApplication();

}