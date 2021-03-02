#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/application.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorApplication : public Application {
    public:
        EditorApplication(const ApplicationSettings &settings) : Application(settings) { }
    protected:
        void OnInitialize() override;
        void OnUpdate(float32 delta_time) override;
    };

}