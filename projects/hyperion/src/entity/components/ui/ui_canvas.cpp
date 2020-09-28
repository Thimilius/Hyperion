#include "hyppch.hpp"

#include "hyperion/entity/components/ui/ui_canvas.hpp"

#include "hyperion/entity/world.hpp"

namespace Hyperion {

    void UICanvas::OnCreate() {
        Component::OnCreate();

        GetWorld()->AddUICanvas(this);
    }

    void UICanvas::OnDestroy() {
        GetWorld()->RemoveUICanvas(this);

        Component::OnDestroy();
    }

    void UICanvas::AddUIGraphic(UIGraphic *ui_graphic) {
        m_ui_graphics.push_back(ui_graphic);
    }

    void UICanvas::RemoveUIGraphic(UIGraphic *ui_graphic) {
        auto begin = m_ui_graphics.begin();
        auto end = m_ui_graphics.end();
        if (std::find(begin, end, ui_graphic) != end) {
            m_ui_graphics.erase(std::remove(begin, end, ui_graphic));
        }
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<UICanvas>(HYP_NAME_OF_TYPE(UICanvas));
}