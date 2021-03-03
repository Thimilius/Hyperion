#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/entity/world.hpp>
#include <hyperion/entity/components/ui/canvas.hpp>
#include <hyperion/entity/components/ui/text.hpp>
#include <hyperion/entity/components/ui/toggle.hpp>
#include <hyperion/rendering/render_driver.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/world_view/editor_world_view_camera_controller.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorWorldView final {
    public:
        inline static World *GetWorld() { return s_editor_world; }
        inline static Camera *GetCamera() { return s_editor_camera; }
        inline static Canvas *GetCanvas() { return s_editor_canvas; }

        static void Initialize();
        static void Update(float32 delta_time);
        static void Render(Rendering::IRenderDriver *render_driver);
    private:
        EditorWorldView() = delete;
        ~EditorWorldView() = delete;
    private:
        static void BuildInterface();
    private:
        inline static World *s_editor_world;
        inline static Camera *s_editor_camera;
        inline static Canvas *s_editor_canvas;

        inline static Text *s_stats_text;
        inline static Toggle *s_grid_toggle;
        inline static Toggle *s_physics_debug_toggle;
        inline static Toggle *s_vsync_toggle;

        inline static bool s_should_draw_grid = true;
        inline static bool s_should_draw_physics_debug = false;

        inline static EditorCameraController *s_editor_camera_controller;
    };

}