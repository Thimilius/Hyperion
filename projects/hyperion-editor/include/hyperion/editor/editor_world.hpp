#pragma once

#include <hyperion/entity/world.hpp>
#include <hyperion/rendering/render_driver.hpp>

namespace Hyperion::Editor {

    class EditorWorld final {
    public:
        inline static World *GetWorld() { return s_editor_world; }

        static void Initialize();
        static void Update(float32 delta_time);
        static void Render(Rendering::IRenderDriver *render_driver, const Rendering::CameraData &camera_data);
    private:
        EditorWorld() = delete;
        ~EditorWorld() = delete;
    private:
        inline static World *s_editor_world;

        inline static bool s_should_draw_grid = true;
        inline static bool s_should_draw_physics_debug = false;
    };

}