#pragma once

#include "hyperion/rendering/material.hpp"
#include "hyperion/rendering/camera.hpp"
#include "hyperion/rendering/mesh.hpp"

namespace Hyperion::Editor {

    class EditorGizmos {
    private:
        Ref<Rendering::Material> m_material;
        Ref<Rendering::Mesh> m_mesh;
        
        f32 m_gizmo_scale = 0.002f;
    public:
        EditorGizmos();

        void Render(const Ref<Rendering::Camera> &camera);
    };

}