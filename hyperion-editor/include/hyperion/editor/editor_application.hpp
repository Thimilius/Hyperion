//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/application.hpp>
#include <hyperion/ecs/world/world.hpp>
#include <hyperion/editor/editor_render_pipeline.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  class EditorApplication : public Application {
  public:
    inline static World *GetWorld() { return s_world; }
    inline static EditorRenderPipeline *GetRenderPipeline() { return s_render_pipeline; }

    static void EnterRuntime();
    static void PauseRuntime();
    static void ExitRuntime();

    static void CreateEntity(EntityPrimitive primitive);
    static void DuplicateEntity();
  protected:
    void OnSetup(ApplicationSettings &settings) override;
    void OnInitialize() override;
    void OnUpdate(float32 delta_time) override;
    void OnTick() override;
  private:
    void OpenWorld();
    void SaveWorld();
  private:
    inline static World *s_world;
    inline static World *s_old_world;

    inline static EditorRenderPipeline *s_render_pipeline;
  };

}
