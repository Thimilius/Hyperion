#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/application.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorApplication : public Application {
    protected:
        void OnSetup(ApplicationSettings& settings) override;
        void OnInitialize() override;
        void OnUpdate(float32 delta_time) override;
    };

}