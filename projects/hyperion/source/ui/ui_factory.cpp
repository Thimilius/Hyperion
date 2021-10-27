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
        button->SetSize(Vector2(150.0f, 40.0f));

        UILabel *label = CreateLabel();
        label->SetText("Button");
        label->SetAnchorPreset(UIAnchorPreset::StretchAll);
        button->GetHierarchy().AddChild(label);

        return button;
    }

}