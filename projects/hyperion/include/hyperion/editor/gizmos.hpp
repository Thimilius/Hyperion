#pragma once

#include "hyperion/core/math/aabb.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/camera.hpp"
#include "hyperion/rendering/mesh.hpp"

namespace Hyperion::Editor {

    class Gizmos {
    private:
        inline static Ref<Rendering::Shader> m_shader;
        inline static Ref<Rendering::Mesh> m_mesh;

        inline static AABB m_x_axis_aabb;
        inline static bool m_gizmo_x_hit = false;
        inline static AABB m_y_axis_aabb;
        inline static bool m_gizmo_y_hit = false;
        inline static AABB m_z_axis_aabb;
        inline static bool m_gizmo_z_hit = false;
    public:
        static void Init();
        static void Update(const Ref<Rendering::Camera> &camera);
        static void Render(const Ref<Rendering::Camera> &camera);
    };

}