#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ui/ui_button.hpp"
#include "hyperion/ui/ui_label.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    class UIFactory final {
    public:
        static UIElement *CreateElement();
        static UIElement *CreateRoot();
        static UILabel *CreateLabel();
        static UIButton *CreateButton();
    private:
        static void Initialize();
        static void Shutdown();
    private:
        UIFactory() = delete;
        ~UIFactory() = delete;
    private:
        friend class Hyperion::Engine;
    };

}