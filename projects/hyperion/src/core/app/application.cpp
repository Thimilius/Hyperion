#include "hyppch.hpp"

#include "hyperion/core/app/application.hpp"

namespace Hyperion {

    Application::Application(const ApplicationSettings &settings) {
        HYP_ASSERT_MESSAGE(!s_instance, "Trying to create application more than once!");
        s_instance = this;

        Engine::SetSettings(settings);
    }

    void Application::Exit() {
        Engine::Exit();
    }

}