#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/components/behaviour.hpp"
#include "hyperion/entity/components/rect_transform.hpp"
#include "hyperion/entity/components/rendering/widget_renderer.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class UiEngine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Widget : public Behaviour, public IEntityMessageListener {
        HYP_REFLECT(Behaviour);
    public:
        inline RectTransform *GetRectTransform() const { return m_rect_transform; }
        inline WidgetRenderer *GetWidgetRenderer() const { return m_widget_renderer; }
        inline bool IsDirty() const { return m_is_dirty; }

        inline Color GetColor() const { return m_color; }
        void SetColor(Color color);

        void OnMessage(EntityMessage message) override;
    protected:
        Widget() = default;
        Widget(const String & name) : Behaviour(name) { }
    protected:
        void SetDirty() { m_is_dirty = true; }
        void ResetDirty() { m_is_dirty = false; }

        void OnCreate() override;
        void OnDestroy() override;

        virtual void OnRebuildMesh(MeshBuilder &mesh_builder) { }
    protected:
        Color m_color = Color::White();
    private:
        WidgetRenderer *m_widget_renderer = nullptr;
        RectTransform *m_rect_transform = nullptr;
        bool m_is_dirty = false;
    private:
        friend class Hyperion::UiEngine;
    }; 

}