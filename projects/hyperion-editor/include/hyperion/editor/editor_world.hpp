#pragma once

#include <hyperion/entity/world.hpp>

namespace Hyperion::Editor {

    class EditorWorld final {
    public:
        inline static World *GetWorld() { return s_editor_world; }
        inline static bool ShouldDrawGrid() { return s_should_draw_grid; }

        static void Initialize();
        static void Update(float32 delta_time);
    private:
        EditorWorld() = delete;
        ~EditorWorld() = delete;
    private:
        inline static World *s_editor_world;

        inline static bool s_should_draw_grid = true;
    };

}