#pragma once

#include "hyperion/editor/gizmos.hpp"
#include "hyperion/editor/camera_controller.hpp"

namespace Hyperion::Editor {

    class EditorEngine {
    private:
        static inline Ref<Rendering::Camera> s_camera;
        static inline Ref<CameraController> s_camera_controller;

        static inline bool s_grid_enabled = false;
        static inline bool s_origin_enabled = false;
    public:
        static void OnInit(const Ref<Rendering::Camera> &camera);
        static void OnUpdate(const Ref<Rendering::Camera> &camera);
        static void OnRender(const Ref<Rendering::Camera> &camera);
        static void OnTick();
    private:
        EditorEngine() = delete;
        ~EditorEngine() = delete;

        static void UpdateTitle();
    };

}