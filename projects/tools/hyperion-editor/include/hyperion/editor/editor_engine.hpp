#pragma once

#include <hyperion/rendering/vertex_array.hpp>
#include <hyperion/rendering/mesh.hpp>
#include <hyperion/rendering/material.hpp>
#include <hyperion/rendering/font.hpp>

#include "hyperion/editor/editor_camera_controller.hpp"
#include "hyperion/editor/editor_gizmo.hpp"

namespace Hyperion::Editor {

    enum class EditorOverlayFlags {
        None         = BIT(0),

        Stats        = BIT(1),

        Grid         = BIT(2),
        Gizmo        = BIT(3),
        Lights       = BIT(4),

        PhysicsDebug = BIT(5),
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(EditorOverlayFlags);

    class EditorEngine {
    private:
        inline static World *s_editor_world;
        inline static World *s_game_world;
        inline static EditorGizmo *s_gizmo;
        
        inline static Camera *s_camera;
        inline static EditorCameraController s_camera_controller;

        inline static u32 s_grid_size = 100;
        inline static u32 s_grid_vertex_count;
        inline static Ref<Rendering::VertexArray> s_grid_vertex_array;

        inline static Ref<Rendering::Mesh> s_icon_mesh;
        inline static Ref<Rendering::Material> s_icon_material;

        inline static String s_stats;
        inline static Ref<Rendering::Font> s_font;

        inline static EditorOverlayFlags s_overlay_flags = EditorOverlayFlags::Stats | EditorOverlayFlags::Grid | EditorOverlayFlags::Gizmo | EditorOverlayFlags::Lights;
    public:
        static Camera *GetCamera() { return s_camera; }

        static void Init();
        static void Update(f32 delta_time);
        static void Render();
        static void Tick();
    private:
        EditorEngine() = delete;
        ~EditorEngine() = delete;

        static void UpdateStats();
        static void InitGridVertexArray();
    };

}