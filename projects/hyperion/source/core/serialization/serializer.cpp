//--------------------- Definition Include ---------------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/serialization/serializer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/serialization/json/json_serialization_stream.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    String Serializer::Serialize(ISerializable *serializable) {
        HYP_ASSERT(serializable);

        JsonSerializationStream stream;
        serializable->Serialize(stream);
        return stream.ToString();
    }

    //--------------------------------------------------------------
    void Serializer::Deserialize(const String &text, ISerializable *serializable) {
        JsonDeserializationStream stream;
        stream.Parse(text);
        serializable->Deserialize(stream);
    }

}