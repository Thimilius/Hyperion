//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/application.hpp>
#include <hyperion/ecs/world/world.hpp>
#include <hyperion/editor/editor_render_pipeline.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_logger.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  class EditorApplication final : public Application {
  public:
    inline static World *GetWorld() { return s_world; }
    inline static EditorRenderPipeline *GetRenderPipeline() { return s_render_pipeline; }
    inline static EditorLogger &GetLogger() { return s_editor_logger; }

    static void EnterRuntime();
    static void PauseRuntime();
    static void ExitRuntime();

    static EntityId CreateEntity(EntityPrimitive primitive);
    static void DuplicateEntity();
    static void DestroyEntity();

    static void NewWorld();
    static void OpenWorld();
    static void SaveWorld();
  protected:
    void OnSetup(ApplicationSettings &settings) override;
    void OnInitialize() override;
    void OnUpdate(float32 delta_time) override;
    void OnShutdown() override;
  private:
    inline static EditorLogger s_editor_logger;
    
    inline static World *s_world;
    inline static World *s_old_world;

    inline static EditorRenderPipeline *s_render_pipeline;
  };

}
