#pragma once

#include "hyperion/core/app/application_layer.hpp"
#include "hyperion/editor/editor_camera_controller.hpp"
#include "hyperion/rendering/vertex_array.hpp"

namespace Hyperion::Editor {

    class EditorLayer : public ApplicationLayer {
    private:
        Ref<Rendering::Camera> m_camera;
        EditorCameraController m_camera_controller;

        u32 m_grid_size = 20;
        u32 m_grid_vertex_count;
        Ref<Rendering::VertexArray> m_grid_vertex_array;

        bool m_grid_enabled = false;
        bool m_origin_enabled = false;
    public:
        Ref<Rendering::Camera> GetCamera() const { return m_camera; }

        void OnAttach() override;
        void OnUpdate(f32 delta_time) override;
        void OnRender() override;
        void OnTick() override;
    private:
        void UpdateTitle();
    };

}