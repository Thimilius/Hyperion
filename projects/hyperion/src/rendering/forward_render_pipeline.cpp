#include "hyppch.hpp"

#include "hyperion/rendering/forward_render_pipeline.hpp"

#include "hyperion/rendering/render_command.hpp"
#include "hyperion/rendering/forward_renderer.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/editor/editor_engine.hpp"

namespace Hyperion::Rendering {

    void ForwardRenderPipeline::Render() {
        RenderCommand::SetViewport(0, 0, Display::GetWidth(), Display::GetHeight());

        World *world = WorldManager::GetActiveWorld();
        WorldEnvironment world_environment = world->GetEnvironment();

        // TODO: Cameras are currently not ordered in any way
        for (Camera *camera : WorldManager::GetActiveWorld()->GetCameras()) {
            CameraClearMode clear_mode = camera->GetClearMode();

            if (clear_mode == CameraClearMode::Color) {
                RenderCommand::Clear(ClearMask::Color | ClearMask::Depth | ClearMask::Stencil, camera->GetBackgroundColor());
            } else if (clear_mode == CameraClearMode::Depth || clear_mode == CameraClearMode::Skybox) {
                RenderCommand::Clear(ClearMask::Depth | ClearMask::Stencil, Color::Black());
            }

            ForwardRenderer::Begin(camera->GetCameraData());
            {
                ForwardRenderer::DrawEntities(world);

                if (clear_mode == CameraClearMode::Skybox) {
                    if (world_environment.skybox) {
                        ForwardRenderer::DrawSkybox(world_environment.skybox);
                    }
                }
            }
            ForwardRenderer::End();
        }

        Editor::EditorEngine::Render();
    }

}