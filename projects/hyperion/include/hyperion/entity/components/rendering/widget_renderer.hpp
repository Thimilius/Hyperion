#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/entity/components/component.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class WidgetRenderer final : public Component {
        HYP_REFLECT();
    public:
        Mesh *GetMesh() const { return m_mesh; }
        void SetMesh(Mesh *mesh) { m_mesh = mesh; }

        Material *GetMaterial() const { return m_material; }
        void SetMaterial(Material *material) { m_material = material; }

        Color GetColor() const { return m_color; }
        void SetColor(Color color) { m_color = color; }
    private:
        WidgetRenderer() : Component("WidgetRenderer") { }
    private:
        uint32 m_depth = 0;

        Mesh *m_mesh = nullptr;
        Material *m_material = nullptr;

        Color m_color = Color::White();
    };

}