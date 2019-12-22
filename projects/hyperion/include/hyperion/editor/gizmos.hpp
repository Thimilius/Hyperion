#pragma once

#include "hyperion/core/math/aabb.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/camera.hpp"
#include "hyperion/rendering/mesh.hpp"

namespace Hyperion::Editor {

    class Gizmos {
    private:
        static inline Ref<Rendering::Shader> m_shader;
        static inline Ref<Rendering::Mesh> m_mesh;

        static inline AABB m_x_axis_aabb;
        static inline bool m_gizmo_x_hit = false;
        static inline AABB m_y_axis_aabb;
        static inline bool m_gizmo_y_hit = false;
        static inline AABB m_z_axis_aabb;
        static inline bool m_gizmo_z_hit = false;
    public:
        static void Init();
        static void Update(const Ref<Rendering::Camera> &camera);
        static void Render(const Ref<Rendering::Camera> &camera);
    };

}