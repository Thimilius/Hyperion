#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/application.hpp>

namespace Sandbox {

    class SandboxApplication : public Hyperion::Application {
    public:
        SandboxApplication(const Hyperion::ApplicationSettings &settings);
    protected:
        void OnInitialize() override;
        void OnUpdate(float32 delta_time) override;
    };

}