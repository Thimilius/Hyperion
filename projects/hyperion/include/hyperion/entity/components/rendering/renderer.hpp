#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/material.hpp"
#include "hyperion/entity/components/behaviour.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Renderer : public Behaviour {
        HYP_REFLECT();
    public:
        Material *GetMaterial() const { return m_material; }
        void SetMaterial(Material *material) { m_material = material; }
    protected:
        Renderer() = default;
        Renderer(const String &name) : Behaviour(name) { }
    private:
        Material *m_material = nullptr;
    };

}