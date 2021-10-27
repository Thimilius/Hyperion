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
        root->SetAnchorPreset(UIAnchorPreset::StretchAll);
        root->GetStyle().SetOpacity(0.0f);
        return root;
    }

    //--------------------------------------------------------------
    UILabel *UIFactory::CreateLabel() {
        UILabel *label = new UILabel();
        label->SetText("New Label");
        label->SetSize(Vector2(150.0f, 40.0f));
        return label;
    }

    //--------------------------------------------------------------
    UIButton *UIFactory::CreateButton() {
        UIButton *button = new UIButton();
        button->GetStyle().GetColorBlock().normal_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        button->GetStyle().GetColorBlock().highlight_color = Color(0.9f, 0.9f, 0.9f, 1.0f);
        button->GetStyle().GetColorBlock().pressed_color = Color(0.75f, 0.75f, 0.75f, 1.0f);
        button->GetStyle().GetColorBlock().disabled_color = Color(0.75f, 0.75f, 0.75f, 0.5f);
        button->SetSize(Vector2(150.0f, 40.0f));

        UILabel *label = CreateLabel();
        label->SetText("Button");
        label->SetAnchorPreset(UIAnchorPreset::StretchAll);
        button->GetHierarchy().AddChild(label);

        return button;
    }

    //--------------------------------------------------------------
    void UIFactory::Initialize() {
        MetaRegistry::Reflect<UIElement>("UIElement");

        MetaRegistry::Reflect<UIButton>("UIButton")
            .Base<UIElement>();

        MetaRegistry::Reflect<UILabel>("UILabel")
            .Base<UIElement>();
    }

    //--------------------------------------------------------------
    void UIFactory::Shutdown() {

    }

}