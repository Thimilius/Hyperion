#pragma once

#include "hyperion/assets/material.hpp"
#include "hyperion/entity/components/behaviour.hpp"

namespace Hyperion {

    class Renderer : public Behaviour {
        HYP_REFLECT(Behaviour);
    public:
        Material *GetMaterial() const { return m_material; }
        void SetMaterial(Material *material) { m_material = material; }
    protected:
        Renderer() = default;
        Renderer(const String &name) : Behaviour(name) { }
    private:
        Material *m_material;
    };

}