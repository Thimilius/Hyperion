#include "hyppch.hpp"

#include "hyperion/rendering/forward_render_pipeline.hpp"

#include "hyperion/rendering/renderer.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/editor/editor_engine.hpp"

namespace Hyperion::Rendering {

    void ForwardRenderPipeline::Render(const Ref<Camera> &camera) {
        RenderEngine::Clear(ClearMask::Color | ClearMask::Depth, Color::Black());

        Renderer::Begin(camera);
        {
            Renderer::DrawWorld(WorldManager::GetActiveWorld());
        }
        Renderer::End();

        Editor::EditorEngine::Render();
    }

}