#pragma once

#include <hyperion/core/math/plane.hpp>
#include <hyperion/entity/components/component.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>

namespace Hyperion::Editor {

    class EditorGizmo : public Component {
        HYP_OBJECT(EditorGizmo, Component);
    public:
        void SetCamera(Camera *camera) { m_camera = camera; }
        void SetSelection(Entity *selection) { m_selection = selection; }
    protected:
        void OnCreate() override;
        void OnUpdate(f32 delta_time) override;
    private:
        void ResetColor();
    private:
        enum class MoveType {
            None,

            XAxis,
            YAxis,
            ZAxis,

            XYAxis,
            XZAxis,
            YZAxis
        };

        MoveType m_move_type = MoveType::None;
        Vec3 m_offset;

        Entity *m_gimzo_x;
        Entity *m_gimzo_y;
        Entity *m_gimzo_z;
        Entity *m_gizmo_xy;
        Entity *m_gizmo_xz;
        Entity *m_gizmo_yz;

        f32 m_gizmo_scale = 0.002f;
        Entity *m_last_gizmo = nullptr;

        Camera *m_camera;
        Entity *m_selection;
    };

}