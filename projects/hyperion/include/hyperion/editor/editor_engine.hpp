#pragma once

#include "hyperion/editor/editor_camera_controller.hpp"
#include "hyperion/rendering/vertex_array.hpp"

namespace Hyperion::Editor {

    class EditorEngine {
    private:
        inline static Ref<Rendering::Camera> s_camera;
        inline static EditorCameraController s_camera_controller;

        inline static u32 s_grid_size = 20;
        inline static u32 s_grid_vertex_count;
        inline static Ref<Rendering::VertexArray> s_grid_vertex_array;

        inline static bool s_overlay_enabled = false;
    public:
        static Ref<Rendering::Camera> GetCamera() { return s_camera; }

        static void Init();
        static void Update(f32 delta_time);
        static void Render();
        static void Tick();
    private:
        static void UpdateTitle();
    };

}