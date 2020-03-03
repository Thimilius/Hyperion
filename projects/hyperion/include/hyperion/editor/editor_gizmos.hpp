#pragma once

#include "hyperion/rendering/material.hpp"
#include "hyperion/rendering/camera_data.hpp"
#include "hyperion/rendering/mesh.hpp"

namespace Hyperion::Editor {

    class EditorGizmos {
    private:
        Ref<Rendering::Material> m_material;
        Ref<Rendering::Mesh> m_mesh;
        
        f32 m_gizmo_scale = 0.002f;
    public:
        void Init();
        void Render(const Rendering::CameraData &camera);
    };

}