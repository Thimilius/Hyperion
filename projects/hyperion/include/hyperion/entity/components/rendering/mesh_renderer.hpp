#pragma once

#include "hyperion/entity/components/rendering/renderer.hpp"

namespace Hyperion {

    class MeshRenderer : public Renderer {
        HYP_REFLECT(Renderer);
    public:
        Mesh *GetMesh() const { return m_mesh; }
        void SetMesh(Mesh *mesh) {
            m_mesh = mesh;
        }
    public:
        MeshRenderer() : Renderer("MeshRenderer") { }
    public:
        static MeshRenderer *Create();
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        Mesh *m_mesh;
    };

}