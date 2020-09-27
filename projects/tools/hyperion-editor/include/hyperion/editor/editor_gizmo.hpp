#pragma once

#include <hyperion/core/color.hpp>
#include <hyperion/core/math/plane.hpp>
#include <hyperion/core/math/vec2.hpp>
#include <hyperion/entity/components/component.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>

#include "hyperion/editor/editor_selection.hpp"

namespace Hyperion::Editor {

    class EditorGizmo : public Component, public IEditorSelectionListener {
        RTTR_ENABLE(Component);
    public:
        enum class GizmoType {
            Position,
            Rotation,
            Scale
        };
    public:
        void SetGizmoType(GizmoType type);
        void SetCamera(Camera *camera) { m_camera = camera; }
    protected:
        void OnCreate() override;
        void OnUpdate(f32 delta_time) override;
    private:
        void SetGizmoMode(Entity *entity, Vec3 position);
        void ResetColor();
        void UpdateScale();

        void OnSelection(Object *selection) override;
    private:
        enum class GizmoMode {
            None,

            PositionXAxis,
            PositionYAxis,
            PositionZAxis,
            PositionXYAxis,
            PositionXZAxis,
            PositionYZAxis,
            PositionXYZAxis,

            RotationXAxis,
            RotationYAxis,
            RotationZAxis,

            ScaleXAxis,
            ScaleYAxis,
            ScaleZAxis,
            ScaleXYZAxis
        };

        GizmoType m_gizmo_type = GizmoType::Position;
        GizmoMode m_gizmo_mode = GizmoMode::None;

        f32 m_gizmo_scale_factor = 0.002f;
        Entity *m_last_gizmo = nullptr;

        Plane m_grabbing_plane;
        Vec3 m_position_offset;
        Vec3 m_rotation_start_angles;
        Vec2 m_rotation_offset;
        Vec3 m_scale_start;
        Vec3 m_scale_offset;
        Quaternion m_rotation_start;
        f32 m_rotation_speed = 0.2f;

        Entity *m_gizmo_position_axis;
        Entity *m_gimzo_position_x;
        Entity *m_gimzo_position_y;
        Entity *m_gimzo_position_z;
        Entity *m_gizmo_position_plane;
        Entity *m_gizmo_position_xy;
        Entity *m_gizmo_position_xz;
        Entity *m_gizmo_position_yz;
        Entity *m_gizmo_position_full;
        Entity *m_gizmo_position_xyz;
        Entity *m_gizmo_rotation;
        Entity *m_gizmo_rotation_x;
        Entity *m_gizmo_rotation_y;
        Entity *m_gizmo_rotation_z;
        Entity *m_gizmo_scale;
        Entity *m_gizmo_scale_x;
        Entity *m_gizmo_scale_y;
        Entity *m_gizmo_scale_z;
        Entity *m_gizmo_scale_full;
        Entity *m_gizmo_scale_xyz;

        Camera *m_camera;
        Entity *m_selection;

        Color m_highlight_color = Color::Yellow();
        Color m_x_axis_color = Color::Red();
        Color m_y_axis_color = Color::Green();
        Color m_z_axis_color = Color::Blue();
        Color m_xyz_axis_color = Color::White();

        const char *POSITION_X_TAG = "Position_X";
        const char *POSITION_Y_TAG = "Position_Y";
        const char *POSITION_Z_TAG = "Position_Z";
        const char *POSITION_XY_TAG = "Position_XY";
        const char *POSITION_XZ_TAG = "Position_XZ";
        const char *POSITION_YZ_TAG = "Position_YZ";
        const char *POSITION_XYZ_TAG = "Position_XYZ";
        const char *ROTATION_X_TAG = "Rotation_X";
        const char *ROTATION_Y_TAG = "Rotation_Y";
        const char *ROTATION_Z_TAG = "Rotation_Z";
        const char *SCALE_X_TAG = "Scale_X";
        const char *SCALE_Y_TAG = "Scale_Y";
        const char *SCALE_Z_TAG = "Scale_Z";
        const char *SCALE_XYZ_TAG = "Scale_XYZ";
    };

}