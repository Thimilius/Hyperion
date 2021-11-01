#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/application.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorApplication : public Application {
    public:
        void EnterRuntime();
        void PauseRuntime();
        void ExitRuntime();
    protected:
        void OnSetup(ApplicationSettings& settings) override;
        void OnInitialize() override;
        void OnUpdate(float32 delta_time) override;
        void OnTick() override;
    private:
        void UpdateStats();
    };

}