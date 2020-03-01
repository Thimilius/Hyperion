#include "hyppch.hpp"

#include "hyperion/rendering/forward_render_pipeline.hpp"

#include "hyperion/rendering/forward_renderer.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/editor/editor_engine.hpp"

namespace Hyperion::Rendering {

    void ForwardRenderPipeline::Render(const CameraData &camera) {
        RenderEngine::Clear(ClearMask::Color | ClearMask::Depth, Color::Black());

        ForwardRenderer::Begin(camera);
        {
            ForwardRenderer::DrawWorld(WorldManager::GetActiveWorld());
        }
        ForwardRenderer::End();

        Editor::EditorEngine::Render();
    }

}