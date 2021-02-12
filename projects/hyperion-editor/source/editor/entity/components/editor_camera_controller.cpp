//--------------------- Definition Include ---------------------
#include "hyperion/editor/entity/components/editor_camera_controller.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entity/entity.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorCameraController::OnCreate() {
        m_camera = GetEntity()->GetComponent<Camera>();
        m_transform = m_camera->GetTransform();
    }

}