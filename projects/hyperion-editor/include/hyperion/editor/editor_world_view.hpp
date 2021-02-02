#pragma once

#include <hyperion/common.hpp>

namespace Hyperion::Editor {

    class EditorWorldView final {
    public:
        inline static bool ShouldDrawGrid() { return s_should_draw_grid; }

        static void OnUpdate(float32 delta_time);
    private:
        EditorWorldView() = delete;
        ~EditorWorldView() = delete;
    private:
        inline static bool s_should_draw_grid = true;
    };

}