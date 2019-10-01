#pragma once

namespace Hyperion {

    class CEngine {
    public:
        template<typename ...Args>
        static void Panic(const TString &system, const TString &message_format, Args... args) {
            TString title = CStringUtils::Format("{} error!", system);
            COperatingSystem::GetInstance()->DisplayError(title, CStringUtils::Format(message_format, args...));

            // This is not really a good way of exiting as it does not necessarily free all relevant resources
            exit(-1);
        }
    private:
        CEngine() = delete;
        ~CEngine() = delete;

        static void Init();
        static void Shutdown();

        friend class CApplication;
    };

}