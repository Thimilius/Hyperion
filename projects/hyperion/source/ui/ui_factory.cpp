//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_factory.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    const ColorBlock SELECTION_COLOR_BLOCK = {
        Color(1.0f, 1.0f, 1.0f, 1.0f),
        Color(0.9f, 0.9f, 0.9f, 1.0f),
        Color(0.75f, 0.75f, 0.75f, 1.0f),
        Color(0.75f, 0.75f, 0.75f, 0.5f)
    };

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
        button->GetStyle().SetColorBlock(SELECTION_COLOR_BLOCK);
        button->SetSize(Vector2(150.0f, 40.0f));

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
        label->SetSize(Vector2(150.0f, 40.0f));
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
        toggle->GetStyle().SetColorBlock(SELECTION_COLOR_BLOCK);
        toggle->SetSize(Vector2(16.0f, 16.0f));
        toggle->SetToggleGraphic(toggle_graphic);
        toggle->SetToggleOnColor(Color::Grey());
        toggle->GetHierarchy().AddChild(toggle_graphic);

        return toggle;
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
    }

}