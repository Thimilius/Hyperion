#pragma once

#include "hyperion/entity/components/component.hpp"

namespace Hyperion::UI {

    class EventSystem final : public Component {
        RTTR_ENABLE(Component);
    public:
        static EventSystem *GetCurrent() { return s_current; }
    protected:
        void OnCreate() override;
        void OnUpdate(f32 delta_time) override;
        void OnDestroy() override;
    private:
        inline static EventSystem *s_current;
    };

}