#include "hyperion/editor/editor_world_view.hpp"

#include <hyperion/core/app/input.hpp>

namespace Hyperion::Editor {

    void EditorWorldView::OnUpdate(float32 delta_time) {
        if (Input::IsKeyDown(KeyCode::F3)) {
            s_should_draw_grid = !s_should_draw_grid;
        }
    }

}