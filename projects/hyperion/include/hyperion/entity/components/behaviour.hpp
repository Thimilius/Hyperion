#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/component.hpp"

//-------------------- Definition Namespace --------------------
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
    private:
        bool m_enabled = true;
    };

}