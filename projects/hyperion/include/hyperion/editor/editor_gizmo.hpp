#pragma once

#include "hyperion/entity/components/component.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"

namespace Hyperion::Editor {

    class EditorGizmo : public Component {
        HYP_OBJECT(EditorGizmo, Component);
    private:
        f32 m_gizmo_scale = 0.002f;

        Entity *m_gimzo_x;
        Entity *m_gimzo_y;
        Entity *m_gimzo_z;

        Camera *m_camera;

        Entity *m_last_entity = nullptr;
    public:
        void SetCamera(Camera *camera) { m_camera = camera; }
    protected:
        void OnCreate() override;
        void OnUpdate(f32 delta_time) override;
    private:
        void ResetColor();
    };

}