#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/ecs/component/components/utilities/camera_controller.hpp>
#include <hyperion/ecs/world/world.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorWorldCamera final {
    public:
        static void Initialize(EntityId camera, World *world);
        static void Update(float32 delta_time, EntityId camera, World *world);
        static void Reset();
    private:
        EditorWorldCamera() = delete;
        ~EditorWorldCamera() = delete;
    private:
        inline static CameraController *s_camera_controller;
    };

}