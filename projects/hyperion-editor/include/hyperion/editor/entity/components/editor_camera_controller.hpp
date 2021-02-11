#pragma once

#include <hyperion/entity/components/component.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>

namespace Hyperion::Editor {

    class EditorCameraController : public Component {
        HYP_REFLECT(Component);
    public:
        void OnCreate() override;
        virtual void OnUpdate(float32 delta_time) = 0;
    protected:
        Camera *m_camera;
        Transform *m_transform;
    };

}