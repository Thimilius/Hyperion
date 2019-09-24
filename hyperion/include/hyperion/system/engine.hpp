#pragma once

namespace Hyperion {

    class CEngine {
    private:
        CEngine() = delete;
        ~CEngine() = delete;

        static void Init();
        static void Shutdown();

        friend class CApplication;
    };

}