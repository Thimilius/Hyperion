#pragma once

#include "hyperion/entity/components/rendering/renderer.hpp"
#include "hyperion/rendering/mesh.hpp"

namespace Hyperion {

    class MeshRenderer : public Renderer {
        HYP_OBJECT(MeshRenderer, Renderer);
    private:
        Ref<Rendering::Mesh> m_shared_mesh;
        mutable Ref<Rendering::Mesh> m_mesh;
        mutable bool m_using_shared_mesh = true;
    public:
        MeshRenderer() : Renderer("MeshRenderer") { }

        inline Ref<Rendering::Mesh> GetSharedMesh() const { return m_shared_mesh; }
        inline void SetSharedMesh(const Ref<Rendering::Mesh> &shared_mesh) { m_shared_mesh = shared_mesh; }

        inline Ref<Rendering::Mesh> GetMesh() const {
            if (m_using_shared_mesh) {
                m_mesh = m_shared_mesh->Copy();
                m_using_shared_mesh = false;
            }
            return m_mesh;
        }
        inline void SetMesh(const Ref<Rendering::Mesh> &material) {
            m_mesh = material;
            m_shared_mesh = material;
            m_using_shared_mesh = false;
        }

        inline Ref<Rendering::Mesh> GetRenderMesh() const {
            return m_using_shared_mesh ? m_shared_mesh : m_mesh;
        }

        BoundingBox GetBounds() const override;
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    };

}