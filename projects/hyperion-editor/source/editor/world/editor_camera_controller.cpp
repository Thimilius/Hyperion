#include "hyperion/editor/world/editor_camera_controller.hpp"

#include <hyperion/entity/entity.hpp>

namespace Hyperion::Editor {

    void EditorCameraController::OnCreate() {
        m_camera = GetEntity()->GetComponent<Camera>();
        m_transform = m_camera->GetTransform();
    }

}