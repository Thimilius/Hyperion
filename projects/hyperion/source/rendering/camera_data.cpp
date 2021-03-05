//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/camera_data.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void CameraData::Serialize(ISerializationStream &stream) {
        stream.WriteInt32("projection_mode", static_cast<int32>(projection_mode));
        stream.WriteVec3("position", position);
        stream.WriteVec3("forward", forward);
        stream.WriteVec3("up", up);
        stream.WriteFloat("near_plane", near_plane);
        stream.WriteFloat("far_plane", far_plane);
        stream.WriteFloat("fov", fov);
        stream.WriteFloat("orthographic_size", orthographic_size);
    }

    //--------------------------------------------------------------
    void CameraData::Deserialize(IDeserializationStream &stream, ReferenceContext &context) {
        projection_mode = static_cast<CameraProjectionMode>(stream.ReadInt32("projection_mode"));
        position = stream.ReadVec3("position");
        forward = stream.ReadVec3("forward");
        up = stream.ReadVec3("up");
        near_plane = stream.ReadFloat("near_plane");
        far_plane = stream.ReadFloat("far_plane");
        fov = stream.ReadFloat("fov");
        orthographic_size = stream.ReadFloat("orthographic_size");
    }

}