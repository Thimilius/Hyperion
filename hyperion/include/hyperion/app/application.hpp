#pragma once

namespace Hyperion {

    class CApplication {
    private:
        static CApplication *s_instance;

        bool m_running;
    public:
        CApplication();
        virtual ~CApplication() = default;

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