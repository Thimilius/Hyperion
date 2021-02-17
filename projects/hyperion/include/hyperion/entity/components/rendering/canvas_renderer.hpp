#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"
#include "hyperion/entity/components/component.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class CanvasRenderer : public Component {
        HYP_REFLECT(Component);
    public:
        Mesh *GetMesh() const { return m_mesh; }
        void SetMesh(Mesh *mesh) { m_mesh = mesh; }
    private:
        CanvasRenderer() : Component("CanvasRenderer") { }
    private:
        static CanvasRenderer *Create();
    private:
        Mesh *m_mesh;
    };

}