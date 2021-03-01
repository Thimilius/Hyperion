#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/rect.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Entity;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    // NOTE: The way the rect transform is implemented is kind of hacky.
    // It replaces the original normal transform when added and if necessary restores it again when destroyed.
    // This feels really weird but the alternative would pretty much be to implement a whole hierarchy only for the ui.
    // We avoid that using the normal entity components.
    // Another benefit should be easier integration with scripting which the normal entity components already handle as well.

    enum class AnchoringPreset {
        Center,

        RightCenter,
        LeftCenter,
        TopCenter,
        BottomCenter,

        TopRightCorner,
        TopLeftCorner,
        BottomRightCorner,
        BottomLeftCorner,

        Stretch,
    };

    class RectTransform final : public Transform {
        HYP_REFLECT(Transform);
    public:
        Rect GetRect() const { return m_rect; }

        Vec2 GetSize() const { return m_size; }
        void SetSize(Vec2 size) {
            m_size = size;
            NotifyTransformChanged();
        }
        Vec2 GetPivot() const { return m_pivot; }
        void SetPivot(Vec2 pivot) {
            m_pivot = pivot;
            NotifyTransformChanged();
        }

        Vec2 GetAnchorMin() const { return m_anchor_min; }
        void SetAnchorMin(Vec2 anchor_min) {
            m_anchor_min = anchor_min;
            NotifyTransformChanged();
        }
        Vec2 GetAnchorMax() const { return m_anchor_max; }
        void SetAnchorMax(Vec2 anchor_max) { 
            m_anchor_max = anchor_max;
            NotifyTransformChanged();
        }

        Vec3 GetAnchoredPosition() const { return m_anchored_position; }
        void SetAnchoredPosition(Vec3 anchored_position) { 
            m_anchored_position = anchored_position;
            NotifyTransformChanged();
        }

        void SetAnchoringPreset(AnchoringPreset anchoring_preset);

        void GetLocalCorners(Vec3 corners[4]) const;
        void GetWorldCorners(Vec3 corners[4]) const;
    protected:
        void OnCreate() override;
        void OnDestroy() override;

        void RecalculateTransform() override;
    private:
        RectTransform() : Transform("RectTransform") { }
    private:
        static RectTransform *Create();
    private:
        Rect m_rect = Rect(-50.0f, -50.0f, 100.0f, 100.0f);
        Vec2 m_size = Vec2(100.0f, 100.0f);
        Vec2 m_pivot = Vec2(0.5f, 0.5f);

        Vec2 m_anchor_min = Vec2(0.5f, 0.5f);
        Vec2 m_anchor_max = Vec2(0.5f, 0.5f);

        Vec3 m_anchored_position = Vec3(0.0f, 0.0f, 0.0f);

        bool m_replace_on_destroy = true;
    private:
        friend class Hyperion::Entity;
    };

    class RectTransformUtility final {
    public:
        static bool RectangleContainsScreenPoint(RectTransform *rect_transform, Vec2 point);
    private:
        RectTransformUtility() = delete;
        ~RectTransformUtility() = delete;
    private:
        static float32 IsLeft(Vec2 p0, Vec2 p1, Vec2 p2);
    };
}