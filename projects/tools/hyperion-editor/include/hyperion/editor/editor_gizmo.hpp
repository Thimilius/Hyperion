#pragma once

#include <hyperion/core/color.hpp>
#include <hyperion/core/math/plane.hpp>
#include <hyperion/entity/components/component.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>

#include "hyperion/editor/editor_selection.hpp"

namespace Hyperion::Editor {

    class EditorGizmo : public Component, public IEditorSelectionListener {
        HYP_OBJECT(EditorGizmo, Component);
    public:
        void SetCamera(Camera *camera) { m_camera = camera; }
    protected:
        void OnCreate() override;
        void OnUpdate(f32 delta_time) override;
    private:
        void ResetColor();
        void UpdateScale();

        void OnSelection(Object *selection) override;
    private:
        enum class MoveType {
            None,

            XAxis,
            YAxis,
            ZAxis,

            XYAxis,
            XZAxis,
            YZAxis,

            XYZAxis
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

        Plane m_grabbing_plane;

        Color m_highlight_color = Color::Yellow();
        Color m_x_axis_color = Color::Red();
        Color m_y_axis_color = Color::Green();
        Color m_z_axis_color = Color::Blue();
        Color m_xyz_axis_color = Color::White();
    };

}