#pragma once

namespace Hyperion {

    class CEngine {
    public:
        static void Panic(const TString &message);
    private:
        CEngine() = delete;
        ~CEngine() = delete;

        static void Init();
        static void Shutdown();

        friend class CApplication;
    };

}