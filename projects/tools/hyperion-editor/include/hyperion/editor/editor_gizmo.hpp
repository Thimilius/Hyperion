#pragma once

#include <hyperion/core/math/plane.hpp>
#include <hyperion/entity/components/component.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>

namespace Hyperion::Editor {

    class EditorGizmo : public Component {
        HYP_OBJECT(EditorGizmo, Component);
    private:
        enum class MoveType {
            None,

            XAxis,
            YAxis,
            ZAxis,
        };

        MoveType m_move_type = MoveType::None;
        Vec3 m_offset;

        Entity *m_gimzo_x;
        Entity *m_gimzo_y;
        Entity *m_gimzo_z;
        f32 m_gizmo_scale = 0.002f;
        Entity *m_last_gizmo = nullptr;

        Camera *m_camera;
    public:
        void SetCamera(Camera *camera) { m_camera = camera; }
    protected:
        void OnCreate() override;
        void OnUpdate(f32 delta_time) override;
    private:
        void ResetColor();
    };

}