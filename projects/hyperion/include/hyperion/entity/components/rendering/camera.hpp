#pragma once

#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/components/component.hpp"
#include "hyperion/rendering/camera_data.hpp"

namespace Hyperion {

    class Camera : public Component, EntityMessageListener {
        HYP_OBJECT(Camera, Component);
    private:
        mutable Rendering::CameraData m_data;
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
        inline f32 GetSize() const { return m_data.size; }
        inline void SetSize(f32 size) { m_data.size = size; }

        inline const Mat4 &GetViewMatrix() const { return m_data.view_matrix; }
        inline const Mat4 &GetProjectionMatrix() const { return m_data.projection_matrix; }
        inline const Mat4 &GetViewProjectionMatrix() const { return m_data.view_projection_matrix; }
        inline const Mat4 &GetInverseViewMatrix() const { return m_data.inverse_view_matrix; }
        inline const Mat4 &GetInverseProjectionMatrix() const { return m_data.inverse_projection_matrix; }

        Rendering::CameraData GetData() const;

        Ray ScreenPointToRay(Vec2 screen_point) const;

        void OnMessage(EntityMessage message);
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        void RecalculateMatricies();
    };

}