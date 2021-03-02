#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/components/behaviour.hpp"
#include "hyperion/entity/components/rect_transform.hpp"
#include "hyperion/entity/components/rendering/widget_renderer.hpp"
#include "hyperion/entity/components/ui/canvas.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Canvas;
    class UiEngine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class EventMessageType {
        PointerEnter,
        PointerExit,
        PointerUp,
        PointerDown,
        PointerClick,
    };

    struct EventMessage {
        EventMessageType type;
    };

    class Widget : public Behaviour, public IEntityMessageListener {
        HYP_REFLECT(Behaviour);
    public:
        inline Color GetColor() const { return m_color; }
        void SetColor(Color color);

        inline RectTransform *GetRectTransform() const { return m_rect_transform; }
        inline WidgetRenderer *GetWidgetRenderer() const { return m_widget_renderer; }
        inline Canvas *GetCanvas() const { return m_canvas; }

        inline int32 GetDepth() const { return m_depth; }
        inline bool IsDirty() const { return m_is_dirty; }

        void OnEntityMessage(EntityMessage message) override;
    protected:
        Widget() = default;
        Widget(const String & name) : Behaviour(name) { }
    protected:
        void SetDirty() { m_is_dirty = true; }
        void ResetDirty() { m_is_dirty = false; }

        void OnCreate() override;
        void OnDestroy() override;

        virtual void OnEventMessage(EventMessage message);

        virtual void OnRebuildMesh(MeshBuilder &mesh_builder) { }
    protected:
        Color m_color = Color::White();
    private:
        RectTransform *m_rect_transform = nullptr;
        WidgetRenderer *m_widget_renderer = nullptr;
        Canvas *m_canvas = nullptr;

        int32 m_depth = -1;
        bool m_is_dirty = false;
    private:
        friend class Hyperion::Canvas;
        friend class Hyperion::UiEngine;
    }; 

}