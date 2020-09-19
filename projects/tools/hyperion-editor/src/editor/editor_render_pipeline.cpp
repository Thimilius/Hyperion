#include "hyperion/editor/editor_render_pipeline.hpp"

#include "hyperion/editor/editor_engine.hpp"
#include "hyperion/rendering/render_command.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorRenderPipeline::Render() {
        RenderCommand::SetViewport(0, 0, Display::GetWidth(), Display::GetHeight());

        RenderCommand::Clear(ClearMask::Color | ClearMask::Depth | ClearMask::Stencil, Color::Black());

        EditorEngine::Render();
    }

}