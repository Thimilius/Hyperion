#pragma once

#include "hyperion/core/math/vec2.hpp"
#include "hyperion/entity/components/transform.hpp"

namespace Hyperion {
    class Entity;
}

namespace Hyperion {

    enum class AnchoringPreset {
        Center,

        RightCenter,
        LeftCenter,
        TopCenter,
        BottomCenter,

        TopRightCorner,
        TopLeftCorner,
        BottomRightCorner,
        BottomLeftCorner
    };

    class RectTransform final : public Transform {
        HYP_REFLECT(Transform);
    public:
        RectTransform() : Transform("RectTransform") { }

        Vec2 GetSize() const { return m_size; }
        void SetSize(Vec2 size) { m_size = size; }
        Vec2 GetPivot() const { return m_pivot; }
        void SetPivot(Vec2 pivot) { m_pivot = pivot; }

        Vec2 GetAnchorMin() const { return m_anchor_min; }
        void SetAnchorMin(Vec2 anchor_min) { m_anchor_min = anchor_min; }
        Vec2 GetAnchorMax() const { return m_anchor_max; }
        void SetAnchorMax(Vec2 anchor_max) { m_anchor_max = anchor_max; }

        Vec3 GetAnchoredPosition() const { return m_anchored_position; }
        void SetAnchoredPosition(Vec3 anchored_position) { m_anchored_position = anchored_position; }

        void SetAnchoringPreset(AnchoringPreset anchoring_preset);

        void GetWorldCorners(Vec3 corners[4]);
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        static RectTransform *Create();
    private:
        Vec2 m_size = Vec2(100, 100);
        Vec2 m_pivot = Vec2(0.0f, 0.0f);

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