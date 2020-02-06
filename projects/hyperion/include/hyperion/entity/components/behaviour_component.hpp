#pragma once

#include "hyperion/entity/components/entity_component.hpp"

namespace Hyperion {

    class BehaviourComponent : public EntityComponent {
        HYP_OBJECT(BehaviourComponent, EntityComponent);
    private:
        bool m_enabled = true;
    public:
        inline bool IsEnabled() const { return m_enabled; }
        inline void SetEnabled(bool enabled) { m_enabled = enabled; }
    protected:
        BehaviourComponent() { }
        BehaviourComponent(const String &name) : EntityComponent(name) {}
        virtual ~BehaviourComponent() = default;
    };

}