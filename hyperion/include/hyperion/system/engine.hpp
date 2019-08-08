#pragma once

namespace Hyperion {

    class CEngine {
    private:
        CEngine() = delete;
        ~CEngine() = delete;

        static void Init();

        friend class CApplication;
    };

}