#pragma once

#include "hyperion/core/color.hpp"
#include "hyperion/entity/components/component.hpp"

namespace Hyperion {

    class UIGraphic : public Component {
        RTTR_ENABLE(Component);
    public:
        inline Color GetColor() const { return m_color; }
        inline void SetColor(Color color) { m_color = color; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        Color m_color = Color::White();
    };

}