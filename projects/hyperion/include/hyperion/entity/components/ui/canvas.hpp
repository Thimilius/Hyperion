 #pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/utilities/mesh_builder.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/entity/components/component.hpp"
#include "hyperion/entity/components/ui/widget.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Widget;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class CanvasScaleMode {
        ScaleWithScreenSize,
        ConstantPixelSize,
    };

    class Canvas : public Component {
        HYP_REFLECT();
    public:
        inline CanvasScaleMode GetScaleMode() const { return m_scale_mode; }
        inline void SetScaleMode(CanvasScaleMode scale_mode) {
            m_scale_mode = scale_mode;
            UpdateScale();
        }
        inline float32 GetScaleFactor() const { return m_scale_factor; }
        inline void SetScaleFactor(float32 scale_factor) {
            m_scale_factor = scale_factor;
            UpdateScale();
        }
        inline Vec2 GetReferenceResoultion() const { return m_reference_resolution; }
        inline void SetReferenceResolution(Vec2 reference_resolution) {
            m_reference_resolution = reference_resolution;
            UpdateScale();
        }

        inline float32 GetScale() const { return m_scale; }

        inline const Vector<Widget *> &GetWidgets() const { return m_widgets; }
    protected:
        void OnCreate() override;
        void OnUpdate(float32 delta_time) override;
        void OnLateUpdate(float32 delta_time) override;
    private:
        Canvas() : Component("Canvas") { }
    private:
        void UpdateScale();
        Vector<Widget *> RaycastWidgets(Vec2 screen_position);
        void SendEventMessage(Widget *widget, EventMessageType type);

        void RegisterWidget(Widget *widget);
        void UnregisterWidget(Widget *widget);

        void UpdateWidgetDepths();
    private:
        CanvasScaleMode m_scale_mode = CanvasScaleMode::ScaleWithScreenSize;

        float32 m_scale = 1.0f;
        float32 m_scale_factor = 1.0f;
        Vec2 m_reference_resolution = Vec2(1280, 720);

        uint32 m_cached_display_width;
        uint32 m_cached_display_height;

        Widget *m_hovered_widget = nullptr;

        MeshBuilder m_mesh_builder;
        Vector<Widget *> m_widgets;
    private:
        friend class Hyperion::Widget;
    };

}