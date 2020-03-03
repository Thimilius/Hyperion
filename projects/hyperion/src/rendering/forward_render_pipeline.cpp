#include "hyppch.hpp"

#include "hyperion/rendering/forward_render_pipeline.hpp"

#include "hyperion/rendering/forward_renderer.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/editor/editor_engine.hpp"

namespace Hyperion::Rendering {

    void ForwardRenderPipeline::Render() {
        for (Camera *camera : WorldManager::GetActiveWorld()->GetCameras()) {
            ForwardRenderer::Begin(camera->GetData());
            {
                ForwardRenderer::DrawWorld(WorldManager::GetActiveWorld());
            }
            ForwardRenderer::End();
        }

        Editor::EditorEngine::Render();
    }

}