//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_engine.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/input.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/entity/components/ui/canvas.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void UiEngine::Update() {
        Canvas *canvas = WorldManager::GetActiveWorld()->FindComponentOfType<Canvas>();
        if (canvas != nullptr) {
            Vector<Widget *> widgets = canvas->GetEntity()->GetComponentsInChildren<Widget>();
            for (Widget *widget : widgets) {
                if (RectTransformUtility::RectangleContainsScreenPoint(widget->GetRectTransform(), Input::GetMousePosition())) {
                    HYP_TRACE("Hover over widget: {}", widget->GetId());
                }
            }
        }
    }

    //--------------------------------------------------------------
    void UiEngine::LateUpdate() {
        Canvas *canvas = WorldManager::GetActiveWorld()->FindComponentOfType<Canvas>();
        if (canvas != nullptr) {
            canvas->UpdateScale();
        }

        for (Widget *widget : s_widgets) {
            if (widget->IsDirty()) {
                s_mesh_builder.Clear();
                widget->OnRebuildMesh(s_mesh_builder);
                widget->ResetDirty();
            }
        }
    }

    //--------------------------------------------------------------
    void UiEngine::RegisterWidget(Widget *widget) {
        HYP_ASSERT(widget);
        HYP_ASSERT(std::find(s_widgets.begin(), s_widgets.end(), widget) == s_widgets.end());

        s_widgets.push_back(widget);
    }

    //--------------------------------------------------------------
    void UiEngine::UnregisterWidget(Widget *widget) {
        HYP_ASSERT(widget);
        HYP_ASSERT(std::find(s_widgets.begin(), s_widgets.end(), widget) != s_widgets.end());

        auto begin = s_widgets.begin();
        auto end = s_widgets.end();
        if (std::find(begin, end, widget) != end) {
            s_widgets.erase(std::remove(begin, end, widget));
        }
    }

}