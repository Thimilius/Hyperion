//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_factory.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  //--------------------------------------------------------------
  UIElement *UIFactory::CreateElement() {
    return new UIElement();
  }

  //--------------------------------------------------------------
  UIElement *UIFactory::CreateRoot() {
    UIElement *root = new UIElement();
    root->SetName("Root");
    root->SetAnchorPreset(AnchorPreset::StretchAll);
    root->GetStyle().SetOpacity(0.0f);
    return root;
  }

  //--------------------------------------------------------------
  UIButton *UIFactory::CreateButton() {
    UIButton *button = new UIButton();
    button->SetName("Button");
    button->SetSize(Vector2(140.0f, 25.0f));

    UILabel *label = CreateLabel();
    label->SetName("Button Label");
    label->SetText("Button");
    label->SetAnchorPreset(AnchorPreset::StretchAll);
    label->GetHierarchy().SetParent(button);

    return button;
  }

  //--------------------------------------------------------------
  UILabel *UIFactory::CreateLabel() {
    UILabel *label = new UILabel();
    label->SetName("Label");
    label->SetText("New Label");
    label->SetSize(Vector2(140.0f, 25.0f));
    return label;
  }

  //--------------------------------------------------------------
  UIToggle *UIFactory::CreateToggle() {
    UIElement *toggle_graphic = CreateElement();
    toggle_graphic->SetName("Toggle Graphic");
    toggle_graphic->SetAnchorPreset(AnchorPreset::StretchAll);
    toggle_graphic->SetAnchorOffsetMin(Vector2(3.0f, 3.0f));
    toggle_graphic->SetAnchorOffsetMax(Vector2(3.0f, 3.0f));

    UIToggle *toggle = new UIToggle();
    toggle->SetName("Toggle");
    toggle->SetSize(Vector2(16.0f, 16.0f));
    toggle->SetToggleGraphic(toggle_graphic);
    toggle->SetToggleOnColor(Color::Grey());
    toggle->GetHierarchy().AddChild(toggle_graphic);

    return toggle;
  }

  //--------------------------------------------------------------
  UIInputField *UIFactory::CreateInputField() {
    UILabel *input_label = CreateLabel();
    input_label->SetName("Input Label");
    input_label->SetText("");
    input_label->SetAnchorPreset(AnchorPreset::StretchAll);
    input_label->SetAnchorOffsetMin(Vector2(3.0f, 3.0f));
    input_label->SetAnchorOffsetMax(Vector2(3.0f, 3.0f));
    input_label->GetStyle().SetColor(Color::Black());
    input_label->SetTextAlignment(TextAlignment::TopLeft);

    UIInputField *input_field = new UIInputField();
    input_field->SetName("Input Field");
    input_field->SetSize(Vector2(140.0f, 25.0f));
    input_field->SetInputLabel(input_label);
    input_field->GetHierarchy().AddChild(input_label);

    return input_field;
  }

  //--------------------------------------------------------------
  void UIFactory::ReflectTypes() {
    MetaRegistry::Reflect<UIElement>("UIElement");

    MetaRegistry::Reflect<UIButton>("UIButton")
      .Base<UIElement>();

    MetaRegistry::Reflect<UILabel>("UILabel")
      .Base<UIElement>();

    MetaRegistry::Reflect<UIToggle>("UIToggle")
      .Base<UIElement>();

    MetaRegistry::Reflect<UIInputField>("UIInputField")
      .Base<UIElement>();
  }

}
