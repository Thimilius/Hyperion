#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/serialization/serializable.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class LightType {
        Directional,
        Point,
        Spot,
    };

    struct LightData : public ISerializable {
        LightType type = LightType::Directional;

        Vec3 position = Vec3(0, 0, 0);
        Vec3 direction = Vec3(0, 0, -1);

        Color color = Color::White();
        float32 intensity = 1.0f;

        float32 range = 10.0f;

        float32 inner_spot_radius = 1.0f;
        float32 outer_spot_radius = 1.0f;

        void Serialize(ISerializationStream &stream) override;
        void Deserialize(IDeserializationStream &stream, ReferenceContext &context) override;
    };

}