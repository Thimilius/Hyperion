//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ui/ui_button.hpp"
#include "hyperion/ui/ui_label.hpp"
#include "hyperion/ui/ui_toggle.hpp"

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
        static UIButton *CreateButton();
        static UILabel *CreateLabel();
        static UIToggle *CreateToggle();
    private:
        static void ReflectTypes();
    private:
        UIFactory() = delete;
        ~UIFactory() = delete;
    private:
        friend class Hyperion::Engine;
    };

}