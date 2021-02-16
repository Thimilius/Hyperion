#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/ui/canvas.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class UiEngine final {
    private:
        UiEngine() = delete;
        ~UiEngine() = delete;
    private:
        static void Update(float32 delta_time);
    private:
        friend class Hyperion::Engine;
    };

}