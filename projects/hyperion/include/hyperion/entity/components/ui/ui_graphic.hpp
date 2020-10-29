#pragma once

#include "hyperion/core/color.hpp"
#include "hyperion/entity/components/component.hpp"

namespace Hyperion {
    class UICanvas;
}

namespace Hyperion {

    class UIGraphic : public Component {
        RTTR_ENABLE(Component);
    public:
        inline Color GetColor() const { return m_color; }
        inline void SetColor(Color color) { m_color = color; }

        inline s32 GetDepth() const { return m_depth; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        Color m_color = Color::White();
        s32 m_depth = -1;
    private:
        friend class Hyperion::UICanvas;
    };

}