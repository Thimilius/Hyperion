//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/data/light_data.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void LightData::Serialize(ISerializationStream &stream) {
        stream.WriteInt32("type", static_cast<int32>(type));
        stream.WriteVec3("position", position);
        stream.WriteVec3("direction", direction);
        stream.WriteColor("color", color);
        stream.WriteFloat("intensity", intensity);
        stream.WriteFloat("range", range);
        stream.WriteFloat("inner_spot_radius", inner_spot_radius);
        stream.WriteFloat("outer_spot_radius", outer_spot_radius);
    }

    //--------------------------------------------------------------
    void LightData::Deserialize(IDeserializationStream &stream, ReferenceContext &context) {
        type = static_cast<LightType>(stream.ReadInt32("type"));
        position = stream.ReadVec3("position");
        direction = stream.ReadVec3("direction");
        color = stream.ReadColor("color");
        intensity = stream.ReadFloat("intensity");
        range = stream.ReadFloat("range");
        inner_spot_radius = stream.ReadFloat("inner_spot_radius");
        outer_spot_radius = stream.ReadFloat("outer_spot_radius");
    }

}