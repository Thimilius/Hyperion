#pragma once

#include "hyperion/entity/components/rendering/renderer.hpp"
#include "hyperion/rendering/mesh.hpp"

namespace Hyperion {

    class MeshRenderer : public Renderer {
        HYP_OBJECT(MeshRenderer, Renderer);
    private:
        Ref<Rendering::Mesh> m_shared_mesh;
    public:
        MeshRenderer() : Renderer("MeshRenderer") { }

        inline Ref<Rendering::Mesh> GetSharedMesh() const { return m_shared_mesh; }
        inline void SetSharedMesh(const Ref<Rendering::Mesh> &shared_mesh) { m_shared_mesh = shared_mesh; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    };

}