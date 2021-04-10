#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"
#include "hyperion/entity/components/rendering/renderer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class MeshRenderer : public Renderer {
        HYP_REFLECT();
    public:
        Mesh *GetMesh() const { return m_mesh; }
        void SetMesh(Mesh *mesh) { m_mesh = mesh; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        MeshRenderer() : Renderer("MeshRenderer") { }
    private:
        Mesh *m_mesh = nullptr;
    };

}