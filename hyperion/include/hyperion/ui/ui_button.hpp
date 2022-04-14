//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ui/ui_element.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  using UIButtonClickCallback = std::function<void()>;

  class UIButton : public UIElement {
    HYP_OBJECT(UIButton);
  public:
    UIButton();
  public:
    void Click();

    void RegisterClickCallback(const UIButtonClickCallback &callback);
    void UnregisterClickCallback();

    virtual void OnEvent(UIEvent &event) override;
  private:
    UIButtonClickCallback m_click_callback;
  };

}
