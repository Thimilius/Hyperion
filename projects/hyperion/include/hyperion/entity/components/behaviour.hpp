#pragma once

#include "hyperion/entity/components/component.hpp"

namespace Hyperion {

    class Behaviour : public Component {
        HYP_OBJECT(Behaviour, Component);
    private:
        bool m_enabled = true;
    public:
        inline bool IsEnabled() const { return m_enabled; }
        inline void SetEnabled(bool enabled) {
            if (m_enabled != enabled) {
                m_enabled = enabled;
                NotifyBehaviourEnabledChanged();
            }
        }
    protected:
        Behaviour() = default;
        Behaviour(const String &name) : Component(name) { }
        virtual ~Behaviour() = default;
    private:
        void NotifyBehaviourEnabledChanged();
    };

}