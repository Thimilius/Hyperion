#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ui/ui_button.hpp"
#include "hyperion/ui/ui_label.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    class UIFactory final {
    public:
        static UIElement *CreateElement();
        static UIElement *CreateRoot();
        static UILabel *CreateLabel();
        static UIButton *CreateButton();
    private:
        UIFactory() = delete;
        ~UIFactory() = delete;
    };

}