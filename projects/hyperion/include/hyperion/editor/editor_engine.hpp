#pragma once

#include "hyperion/editor/gizmos.hpp"
#include "hyperion/editor/camera_controller.hpp"

namespace Hyperion::Editor {

    class EditorEngine {
    private:
        inline static Ref<Rendering::Camera> s_camera;
        inline static Ref<CameraController> s_camera_controller;

        inline static u32 s_grid_size = 20;
        inline static u32 s_grid_vertex_count;
        inline static Ref<Rendering::VertexArray> s_grid_vertex_array;

        inline static bool s_grid_enabled = false;
        inline static bool s_origin_enabled = false;
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