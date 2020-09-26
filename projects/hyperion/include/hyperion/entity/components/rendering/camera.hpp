#pragma once

#include "hyperion/core/layer.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/components/component.hpp"
#include "hyperion/rendering/camera_data.hpp"

namespace Hyperion {

    enum class CameraClearMode {
        Skybox,
        Color,
        Depth,
        Nothing
    };

    class Camera final : public Component, public IEntityMessageListener {
        HYP_OBJECT(Camera, Component);
    public:
        Camera() : Component("Camera") { }
        
        inline Rendering::CameraMode GetMode() const { return m_data.mode; }
        inline void SetMode(Rendering::CameraMode type) { m_data.mode = type; }

        inline f32 GetNearPlane() const { return m_data.near_plane; }
        inline void SetNearPlane(f32 near_plane) { m_data.near_plane = near_plane; }
        inline f32 GetFarPlane() const { return m_data.far_plane; }
        inline void SetFarPlane(f32 far_plane) { m_data.far_plane = far_plane; }

        inline f32 GetFOV() const { return m_data.fov; }
        inline void SetFOV(f32 fov) { m_data.fov = fov; }
        inline f32 GetOrthographicSize() const { return m_data.orthographic_size; }
        inline void SetOrthographicSize(f32 orthographic_size) { m_data.orthographic_size = orthographic_size; }

        inline const Mat4 &GetViewMatrix() const { return m_data.view_matrix; }
        inline const Mat4 &GetProjectionMatrix() const { return m_data.projection_matrix; }
        inline const Mat4 &GetViewProjectionMatrix() const { return m_data.view_projection_matrix; }
        inline const Mat4 &GetInverseViewMatrix() const { return m_data.inverse_view_matrix; }
        inline const Mat4 &GetInverseProjectionMatrix() const { return m_data.inverse_projection_matrix; }

        inline LayerMask GetCullingMask() const { return m_culling_mask; }
        inline void SetCullingMask(LayerMask culling_mask) { m_culling_mask = culling_mask; }
        inline CameraClearMode GetClearMode() const { return m_clear_mode; }
        inline void SetClearMode(CameraClearMode clear_mode) { m_clear_mode = clear_mode; }
        inline Color GetBackgroundColor() const { return m_background_color; }
        inline void SetBackgroundColor(Color background_color) { m_background_color = background_color; }

        Rendering::CameraData GetCameraData() const;

        Ray ScreenPointToRay(Vec2 screen_point) const;

        void OnMessage(EntityMessage message);
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        void RecalculateMatricies();
    private:
        mutable Rendering::CameraData m_data;

        LayerMask m_culling_mask = LayerMask::Everything;
        CameraClearMode m_clear_mode = CameraClearMode::Color;
        Color m_background_color = Color::Black();
    };

}