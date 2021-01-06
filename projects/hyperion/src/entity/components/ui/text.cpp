#include "hyppch.hpp"

#include "hyperion/entity/components/ui/text.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::UI {

    void Text::SetFontSize(u32 font_size) {
        // TODO: Implement
    }

    void Text::OnCreate() {
        Widget::OnCreate();

        m_font = Font::Create("data/fonts/robotomono_regular.ttf", m_font_size, FontCharacterSet::All);
    }

}

HYP_REFLECT_REGISTER_BEGIN
{
    registration::class_<UI::Text>("Text");
}
HYP_REFLECT_REGISTER_END