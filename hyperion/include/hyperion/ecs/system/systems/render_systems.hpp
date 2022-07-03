//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion { namespace Rendering {
    struct RenderObjectContextUIElement;
  }

  namespace UI {
    class UIElement;
    struct UIViewComponent;
  }}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class MeshBoundsSystem final : public ISystem {
  public:
    void Run(EntityManager *manager) override;
  };

  class EnvironmentSystem final : public ISystem {
  public:
    void Run(EntityManager *manager) override;
  };

  class CameraSystem final : public ISystem {
  public:
    void Run(EntityManager *manager) override;
  };

  class LightSystem final : public ISystem {
  public:
    void Run(EntityManager *manager) override;
  };

  class SpriteRenderSystem final : public ISystem {
  public:
    void Run(EntityManager *manager) override;
  };

  class MeshRenderSystem final : public ISystem {
  public:
    void Run(EntityManager *manager) override;
  };

  class UIRenderSystem final : public ISystem {
  public:
    void Run(EntityManager *manager) override;
  public:
    static void Run(UI::UIViewComponent *ui_view, Delegate<RenderObjectContextUIElement &()> ui_object_adder);
  private:
    static void RenderElement(UI::UIElement *element, Delegate<RenderObjectContextUIElement &()> ui_object_adder);
  };

}
