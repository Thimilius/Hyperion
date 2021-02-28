#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/serialization/serialization_stream.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class ISerializable {
    public:
        virtual ~ISerializable() = default;
    public:
        virtual void Serialize(SerializationStream &serialization_stream) = 0;
        virtual void Deserialize(SerializationStream &serialization_stream) = 0;
    };

}