#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/entity/components/component.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class WidgetRenderer : public Component {
        HYP_REFLECT(Component);
    public:
        Mesh *GetMesh() const { return m_mesh; }
        void SetMesh(Mesh *mesh) { m_mesh = mesh; }

        Material *GetMaterial() const { return m_material; }
        void SetMaterial(Material *material) { m_material = material; }
    private:
        WidgetRenderer() : Component("WidgetRenderer") { }
    private:
        static WidgetRenderer *Create();
    private:
        Mesh *m_mesh = nullptr;
        Material *m_material = nullptr;
    };

}