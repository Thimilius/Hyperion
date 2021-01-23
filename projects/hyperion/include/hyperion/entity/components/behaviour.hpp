#pragma once

#include "hyperion/entity/components/component.hpp"

namespace Hyperion {

    class Behaviour : public Component {
        HYP_REFLECT(Component);
    public:
        inline bool IsEnabled() const { return m_enabled; }
        virtual inline void SetEnabled(bool enabled) { m_enabled = enabled; }
        bool IsActiveAndEnabled();
    protected:
        Behaviour() = default;
        Behaviour(const String &name) : Component(name) { }
        virtual ~Behaviour() override = default;
    private:
        bool m_enabled = true;
    };

}