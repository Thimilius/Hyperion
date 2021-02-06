#pragma once

#include "hyperion/assets/material.hpp"
#include "hyperion/entity/components/component.hpp"

namespace Hyperion {

    class Renderer : public Component {
        HYP_REFLECT(Component);
    public:
        Material *GetMaterial() const { return m_material; }
        void SetMaterial(Material *material) { m_material = material; }
    protected:
        Renderer() = default;
        Renderer(const String &name) : Component(name) { }
    private:
        Material *m_material;
    };

}