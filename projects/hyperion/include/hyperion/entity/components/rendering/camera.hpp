#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/layer.hpp"
#include "hyperion/entity/components/behaviour.hpp"
#include "hyperion/rendering/camera_data.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class CameraClearMode {
        Skybox,
        Color,
        Depth,
        Nothing
    };

    class Camera final : public Behaviour, public IEntityMessageListener {
        HYP_REFLECT(Behaviour);
    public:
        Camera() : Behaviour("Camera") { }

        inline Rendering::CameraProjectionMode GetProjectionMode() const { return m_data.projection_mode; }
        inline void SetProjectionMode(Rendering::CameraProjectionMode type) { m_data.projection_mode = type; }

        inline float32 GetNearPlane() const { return m_data.near_plane; }
        inline void SetNearPlane(float32 near_plane) { m_data.near_plane = near_plane; }
        inline float32 GetFarPlane() const { return m_data.far_plane; }
        inline void SetFarPlane(float32 far_plane) { m_data.far_plane = far_plane; }

        inline float32 GetFOV() const { return m_data.fov; }
        inline void SetFOV(float32 fov) { m_data.fov = fov; }
        inline float32 GetOrthographicSize() const { return m_data.orthographic_size; }
        inline void SetOrthographicSize(float32 orthographic_size) { m_data.orthographic_size = orthographic_size; }

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

        Vec3 ScreenToWorldPoint(Vec3 screen_point) const;
        Vec2 WorldToScreenPoint(Vec3 world_point) const;
        Ray ScreenPointToRay(Vec2 screen_point) const;

        void OnMessage(EntityMessage message) override;
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        void RecalculateMatricies();
    private:
        static Camera *Create();
    private:
        mutable Rendering::CameraData m_data;

        LayerMask m_culling_mask = LayerMask::Everything;
        CameraClearMode m_clear_mode = CameraClearMode::Color;
        Color m_background_color = Color::Black();
    };

}