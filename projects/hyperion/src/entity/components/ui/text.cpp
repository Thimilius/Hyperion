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

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;
    using namespace Hyperion::UI;

    registration::class_<Text>(HYP_NAME_OF_TYPE(Text));
}