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
        for (World *world : WorldManager::GetWorlds()) {
            Canvas *canvas = world->FindComponentOfType<Canvas>();
            UpdateCanvas(canvas);
        }
    }

    //--------------------------------------------------------------
    void UiEngine::LateUpdate() {
        for (World *world : WorldManager::GetWorlds()) {
            Canvas *canvas = world->FindComponentOfType<Canvas>();
            if (canvas != nullptr) {
                canvas->UpdateScale();
            }
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
    void UiEngine::UpdateCanvas(Canvas *canvas) {
        if (canvas == nullptr) {
            return;
        }

        Vector<Widget *> hovered_widgets = RaycastWidgets(canvas->GetWidgets(), Input::GetMousePosition());

        if (hovered_widgets.size() > 0) {
            Widget *hovered_widget = hovered_widgets[0];

            if (s_hovered_widget != hovered_widget) {
                if (s_hovered_widget != nullptr) {
                    SendEventMessage(s_hovered_widget, EventMessageType::PointerExit);
                }
                s_hovered_widget = hovered_widget;

                SendEventMessage(s_hovered_widget, EventMessageType::PointerEnter);
            }
        } else {
            if (s_hovered_widget) {
                // Send exit to previous hovered widget.
                SendEventMessage(s_hovered_widget, EventMessageType::PointerExit);
                s_hovered_widget = nullptr;
            }
        }

        if (s_hovered_widget) {
            if (Input::IsMouseButtonUp(MouseButtonCode::Left)) {
                SendEventMessage(s_hovered_widget, EventMessageType::PointerClick);
            }
        }
    }

    //--------------------------------------------------------------
    Vector<Widget *> UiEngine::RaycastWidgets(const Vector<Widget *> &widgets, Vec2 screen_position) {
        Vector<Widget *> result;
        for (Widget *widget : widgets) {
            if (!RectTransformUtility::RectangleContainsScreenPoint(widget->GetRectTransform(), screen_position)) {
                continue;
            }

            if (!widget->IsRaycastTarget()) {
                continue;
            }

            result.push_back(widget);
        }

        std::sort(result.begin(), result.end(), [](Widget *lhs, Widget *rhs) {
            return lhs->GetDepth() > rhs->GetDepth();
        });

        return result;
    }

    //--------------------------------------------------------------
    void UiEngine::SendEventMessage(Widget *widget, EventMessageType type) {
        widget->OnEventMessage({ type });
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

        // We have to potentially kill our current hovered entity.
        if (widget == s_hovered_widget) {
            s_hovered_widget = nullptr;
        }

        auto begin = s_widgets.begin();
        auto end = s_widgets.end();
        if (std::find(begin, end, widget) != end) {
            s_widgets.erase(std::remove(begin, end, widget));
        }
    }

}