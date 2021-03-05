#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/mat4.hpp"
#include "hyperion/core/math/ray.hpp"
#include "hyperion/core/serialization/serializable.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class CameraProjectionMode {
        Perspective,
        Orthographic
    };

    struct CameraData : public ISerializable {
        CameraProjectionMode projection_mode = CameraProjectionMode::Perspective;

        Vec3 position = Vec3(0, 0, 0);
        Vec3 forward = Vec3(0, 0, -1);
        Vec3 up = Vec3(0, 1, 0);

        float32 near_plane = 0.1f;
        float32 far_plane = 1000.0f;

        float32 fov = 90.0f;
        float32 orthographic_size = 2.75f;

        Mat4 view_matrix = Mat4::Identity();
        Mat4 projection_matrix = Mat4::Identity();
        Mat4 view_projection_matrix = Mat4::Identity();

        Mat4 inverse_view_matrix = Mat4::Identity();
        Mat4 inverse_projection_matrix = Mat4::Identity();
        Mat4 inverse_view_projection_matrix = Mat4::Identity();

        void Serialize(ISerializationStream &stream) override;
        void Deserialize(IDeserializationStream &stream, ReferenceContext &context) override;
    };

}